#pragma once
#include <queue>
#include <memory>
#include <mutex>
#include <cstring>
#include <functional>
#include <concurrentqueue/concurrentqueue.h>
#include <utils/noncopyable.hpp>
#include <utils/singleton.hpp>
#include <utils/spin_lock.hpp>
#include <net/decl.h>

template <int size>
class data_buffer : public basic_data_buffer ,std::enable_shared_from_this<data_buffer<size>>, noncopyable
{
public:
	data_buffer()
		:alloc_size_(size)
		,total_length_(0)
		,header_length_(0)
	{

	}

	virtual ~data_buffer()
	{
		
	}

	virtual char* total_buffer() 
	{
		return buffer_;
	}

	virtual size_t total_length()
	{
		return total_length_;
	}

	virtual void set_total_length(size_t total_length)
	{
		total_length_ = total_length;
	}

	virtual void set_header_length(size_t header_length)
	{
		header_length_ = header_length;
	}

	virtual char* body_buffer()
	{
		return buffer_ + header_length_;
	}

	virtual size_t body_length()
	{
		return total_length_ - header_length_;
	}

	virtual void release() 
	{
		total_length_ = 0;
		header_length_ = 0;
	}
	virtual size_t copy_from(basic_data_buffer* src)
	{
		return copy_from(src->total_buffer(), src->total_length());
	}
	virtual size_t copy_from(char* src, size_t src_len) 
	{
		if (alloc_size_ < src_len)
		{
			assert(false);
			return 0;
		}
		std::memcpy(buffer_, src, src_len);
		total_length_ = src_len;
		return total_length_;
	}

	virtual size_t copy_to(char* desc, size_t dest_len) 
	{
		if (dest_len < total_length_) 
		{
			assert(false);
			return 0;
		}
		std::memcpy(desc, buffer_, dest_len);
		return dest_len;
	}
private:
	size_t		alloc_size_;
	size_t		total_length_;
	size_t		header_length_;
	char		buffer_[size];
};

template<int size>
class buffer_pool
{
	using lock = spin_lock;
public:
	buffer_pool() {}
	~buffer_pool()
	{
		release();
	}
	buffer_ptr alloc() 
	{
		/*std::unique_lock<lock> guard(mutex_);
		basic_data_buffer* buffer = nullptr;
		if (free_queue_.empty())
		{
			buffer = new data_buffer<size>();
		}
		else 
		{
			buffer = free_queue_.front();
			free_queue_.pop();
		}
		guard.unlock();*/
		basic_data_buffer* buffer = nullptr;
		if(!free_queue_.try_dequeue(buffer))
		{
			buffer = new data_buffer<size>();
		}
		//在智能指针释放的时候会还到buffer_pool中来，但是如果还到buffer_pool的时候buffer_pool已经被析构可能会崩溃(暂时不处理)
		buffer_ptr ptr(buffer, std::bind(&buffer_pool::return_to_pool, this, std::placeholders::_1));
		return ptr;
		return nullptr;
	}
	
	void release()
	{
		/*std::unique_lock<lock> guard(mutex_);
		while (!free_queue_.empty())
		{
			auto buffer = free_queue_.front();
			free_queue_.pop();
			delete buffer;
		}
		guard.unlock();*/
		basic_data_buffer* buffer = nullptr;
		while (free_queue_.try_dequeue(buffer))
		{
			delete buffer;
		}
	}
protected:
	void return_to_pool(basic_data_buffer* buffer)
	{
		buffer->release();
		free_queue_.enqueue(buffer);
		/*std::unique_lock<lock> guard(mutex_);
		free_queue_.push(buffer);
		guard.unlock();*/
	}
private:
	/*std::queue<basic_data_buffer*> free_queue_;
	lock mutex_;*/

	moodycamel::ConcurrentQueue<basic_data_buffer*> free_queue_;
};

class buffer_pool_manager : public singleton<buffer_pool_manager>
{
public:
	buffer_ptr alloc(int size)
	{
		if(size<64)
		{
			return buffer_pool64B_.alloc();
		}
		if (size < 128)
		{
			return buffer_pool128B_.alloc();
		}
		if (size < 256)
		{
			return buffer_pool256B_.alloc();
		}
		if (size < 512)
		{
			return buffer_pool512B_.alloc();
		}
		if (size < 1024)
		{
			return buffer_pool1K_.alloc();
		}
		if (size < 2048)
		{
			return buffer_pool2K_.alloc();
		}
		if (size < 4096)
		{
			return buffer_pool4K_.alloc();
		}
		if (size < 8192)
		{
			return buffer_pool8K_.alloc();
		}
		if (size < 16384)
		{
			return buffer_pool16K_.alloc();
		}
		if (size < 32768)
		{
			return buffer_pool32K_.alloc();
		}
		if (size < 65536)
		{
			return buffer_pool64K_.alloc();
		}
		return nullptr;
	}
private:
	buffer_pool<64>		buffer_pool64B_;		//管理64B的内存池
	buffer_pool<128>	buffer_pool128B_;		//管理128B的内存池
	buffer_pool<256>	buffer_pool256B_;		//管理256B的内存池
	buffer_pool<512>	buffer_pool512B_;		//管理512B的内存池
	buffer_pool<1024>	buffer_pool1K_;			//管理1k的内存池
	buffer_pool<2048>	buffer_pool2K_;			//管理2k的内存池
	buffer_pool<4096>	buffer_pool4K_;			//管理4k的内存池
	buffer_pool<8192>	buffer_pool8K_;			//管理8k的内存池
	buffer_pool<16384>	buffer_pool16K_;		//管理16k的内存池
	buffer_pool<32768>	buffer_pool32K_;		//管理32k的内存池
	buffer_pool<65536>	buffer_pool64K_;		//管理64k的内存池
};
