#pragma once
#include <memory>
#include <asio.hpp>
#include <logger/logger.h>
#include <utils/noncopyable.hpp>
#include <utils/platform.hpp>
#include <utils/spin_lock.hpp>
#include <net/buffer_stream.h>
#include <net/buffer_pool.h>
#include <net/decl.h>
#include <net/config.h>
#include <net/context.h>

template<class this_parser>//, typename std::enable_if_t<std::is_convertible_v<this_parser*, basic_parser*>>* = nullptr>
class EngineAPI tcp_session : public basic_session
{
	using parser = this_parser;
	using lock = spin_lock;
	static_assert(std::is_convertible_v<this_parser*, basic_parser*>, "this_parser must be inherit basic_parser!");
public:
	explicit tcp_session(asio::io_context& context, int64_t session_id, data_handler_ptr data_handler);
	~tcp_session();
	virtual int64_t sid();
	asio::ip::tcp::socket& socket();
	void close();
	bool connect(const std::string& ip, int port);
	bool async_connect(const std::string& ip, int port);
	virtual void send(const char* data, std::size_t length);
	virtual void send(const buffer_ptr& buffer);
	virtual void start();
protected:
	void do_recv();
	void do_send();
private:
	asio::io_context& context_;
	asio::ip::tcp::socket socket_;
	int64_t session_id_;
	char recv_buffer_[max_recv_buffer_size] = {0};
	std::string send_buffer_;
	std::string cache_buffer_;
	bool sending_ = false;
	lock write_mutex_;
	parser parser_;
	data_handler_ptr data_handler_;
};


template<class this_parser>
tcp_session<this_parser>::tcp_session(asio::io_context& context, int64_t session_id, data_handler_ptr data_handler)
	:context_(context)
	, socket_(context)
	, session_id_(session_id)
	, data_handler_(data_handler)
{
	send_buffer_.reserve(1024);
	cache_buffer_.reserve(1024);
}

template<class this_parser>
tcp_session<this_parser>::~tcp_session()
{
	//close();
}

template<class this_parser>
int64_t tcp_session<this_parser>::sid()
{
	return session_id_;
}

template<class this_parser>
asio::ip::tcp::socket& tcp_session<this_parser>::socket()
{
	return socket_;
}

template<class this_parser>
void tcp_session<this_parser>::close()
{
	auto self(this->shared_from_this());
	asio::post(context_, std::bind([this, self]()
		{
			if (!socket_.is_open())
			{
				return;
			}
			std::error_code ec;
			socket_.shutdown(asio::ip::tcp::socket::shutdown_type::shutdown_both, ec);
			if (ec)
			{
				Log_Error("tcp_session::close error_code : {} , message : {}", ec.value(), ec.message());
			}
			socket_.close(ec);
			data_handler_->on_close(this->shared_from_this());
		}));
}

template<class this_parser>
bool tcp_session<this_parser>::connect(const std::string& ip, int port)
{
	if (socket_.is_open())
	{
		return false;
	}
	std::error_code ec;
	asio::ip::tcp::resolver resolver(context_);
	auto endpoint = resolver.resolve(ip, std::to_string(port), ec);
	if (ec)
	{
		close();
		Log_Error("tcp_session::connect resolver error_code : {} , message : {}", ec.value(), ec.message());
		return false;
	}
	auto end_point = endpoint.begin()->endpoint();
	socket_.connect(end_point, ec);
	if (ec)
	{
		close();
		Log_Error("tcp_session::connect  error_code : {} , message : {}", ec.value(), ec.message());
		return false;
	}
	start();
	return true;
}

template<class this_parser>
bool tcp_session<this_parser>::async_connect(const std::string& ip, int port)
{
	if (socket_.is_open())
	{
		return false;
	}
	std::error_code ec;
	asio::ip::tcp::resolver resolver(context_);
	auto endpoint = resolver.resolve(ip, std::to_string(port), ec);
	if (ec)
	{
		close();
		Log_Error("tcp_session::connect resolver error_code : {} , message : {}", ec.value(), ec.message());
		return false;
	}
	auto end_point = endpoint.begin()->endpoint();
	auto self(shared_from_this());
	socket_.async_connect(end_point, [this, self](std::error_code ec) {
		if (ec)
		{
			close();
			Log_Error("tcp_session::connect failed error_code : {} , message : {}", ec.value(), ec.message());
			return;
		}
		start();
		});
	return true;
}

template<class this_parser>
void tcp_session<this_parser>::send(const char* data, std::size_t length)
{
	std::unique_lock<lock> guard(write_mutex_);
	send_buffer_.append(std::string_view(data, length));
	guard.unlock();
	auto self(this->shared_from_this());
	asio::post(context_, std::bind([this, self]() {
		if (this->sending_) return;
		do_send();
		}));
}

template<class this_parser>
void tcp_session<this_parser>::send(const buffer_ptr& buffer)
{
	auto self(this->shared_from_this());
	asio::post(context_, std::bind([this, self,buffer]() {
		send_buffer_.append(std::string_view(buffer->total_buffer(), buffer->total_length()));
		if (this->sending_) return;
		do_send();
		}));
}

template<class this_parser>
void tcp_session<this_parser>::start()
{
	data_handler_->on_connect(this->shared_from_this());
	do_recv();
}


template<class this_parser>
void tcp_session<this_parser>::do_recv()
{
	auto self(shared_from_this());
	socket_.async_read_some(asio::buffer(recv_buffer_, max_recv_buffer_size),
		[this, self](std::error_code ec, std::size_t bytes_transferred)
		{
			if (!ec)
			{
				auto ret_code = parser_.recv_and_decode(this->shared_from_this(), data_handler_, recv_buffer_, bytes_transferred);
				if (ret_code > parser_code::parse_wait)
				{
					Log_Error("tcp_session parser error code:{} ,  message:{}", ec.value(), ec.message());
					close();
				}
				do_recv();
			}
			else
			{
				Log_Error("tcp_session do_recv error code:{} ,  message:{}", ec.value(), ec.message());
				close();
			}
		});
}

template<class this_parser>
void tcp_session<this_parser>::do_send()
{
	if (!socket_.is_open())
	{
		return;
	}
	std::unique_lock<lock> guard(write_mutex_);
	send_buffer_.swap(cache_buffer_);
	guard.unlock();
	if (cache_buffer_.empty())
	{
		sending_ = false;
		return;
	}
	sending_ = true;
	auto self(shared_from_this());
	asio::async_write(socket_, asio::buffer(cache_buffer_.data(), cache_buffer_.size()), [this, self](std::error_code ec, std::size_t bytes_transferred)
		{
			sending_ = false;
			if (!ec)
			{
				cache_buffer_.clear();
				{
					std::unique_lock<lock> guard(write_mutex_);
					if (send_buffer_.empty()) return;
					guard.unlock();
				}
				do_send();
			}
			else
			{
				Log_Error("tcp_session do_send error code:{} ,  message:{}", ec.value(), ec.message());
				close();
			}
		});
}

