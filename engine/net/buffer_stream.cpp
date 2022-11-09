#include <net/buffer_stream.h>
buffer_stream::buffer_stream(int init_size /*= 1024*/)
    :array_(nullptr)
    ,write_(0)
    ,read_(0)
    ,size_(init_size)
{
    array_ = static_cast<char*>(std::malloc(init_size));
}

buffer_stream::~buffer_stream() 
{
    std::free(array_);
}

int32_t buffer_stream::append(const void* data, int32_t size)
{
    return append(static_cast<const char*>(data), size);
}

int32_t buffer_stream::append(const char* data, int32_t size)
{
    int need = size - writeable_length();
    if (need > 0)
    {
        //todo 此处申请2倍原来的长度,后续考虑要不要用jmalloc
        array_ = static_cast<char*>(std::realloc(array_, this->size_ + this->size_));
        assert(array_);
        this->size_ += this->size_;
    }
    std::memcpy(array_ + write_, data, size);
    write_ += size;
    return size;
}

uint32_t buffer_stream::capacity() const 
{ 
    return size_; 
}

uint32_t buffer_stream::writeable_length() const 
{ 
    return size_ - write_; 
}

uint32_t buffer_stream::readable_length() const 
{ 
    return write_ - read_; 
}

char* buffer_stream::write_addr() const 
{ 
    return array_ + write_; 
}

char* buffer_stream::read_addr() const 
{ 
    return array_ + read_; 
}

void buffer_stream::offset_write(int32_t size) 
{ 
    write_ += size; 
}

void buffer_stream::offset_read(int32_t size) 
{ 
    read_ += size; 
}

void buffer_stream::retrieve()
{
    if (!read_) return;
    int32_t len = readable_length();
    if (read_)
    {
        std::memmove(array_, read_addr(), len);
    }
    read_ = 0;
    write_ = len;
}
