#pragma once
#include <cassert>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <utils/noncopyable.hpp>
#include <utils/platform.hpp>
class EngineAPI buffer_stream : noncopyable
{
public:
    buffer_stream(int init_size = 1024);
    ~buffer_stream();
    int32_t append(const void* data, int32_t size);
    int32_t append(const char* data, int32_t size);
    uint32_t capacity() const;
    uint32_t writeable_length() const;
    uint32_t readable_length() const;
    char* write_addr() const;
    char* read_addr() const;
    void offset_write(int32_t size);
    void offset_read(int32_t size);
    void retrieve();
private:
    char* array_;
    uint32_t write_;
    uint32_t read_;
    uint32_t size_;
};
