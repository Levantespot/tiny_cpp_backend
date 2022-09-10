#ifndef TCB_BASE_LOGSTREAM_H
#define TCB_BASE_LOGSTREAM_H

#include "noncopyable.h"
// #include <vector>
#include <string>
#include <cstring> // memcpy

namespace TCB
{

const size_t kSmallBuffer = 4000;
const size_t kLargeBuffer = kSmallBuffer*1000;

template<size_t SIZE>
class FixedBuffer : noncopyable
{
private:
    char data_[SIZE];
    char* cur_;
public:
    FixedBuffer() : cur_(data_) {}
    // 返回缓存的头指针
    const char* get_data() const { return data_; }
    // 写缓存
    void append(const char* buf, size_t len);
    // 返回缓存的大小
    size_t capacity() const { return SIZE; }
    // 返回缓存使用量
    size_t size() const { return static_cast<size_t>(cur_-data_); }
    // 返回缓存剩余量
    size_t avail() const { return SIZE - size(); }
    // 重置
    void reset() { cur_ = data_; }
}; // class FixedBuffer

class LogStream : noncopyable
{
    using Buffer = FixedBuffer<kSmallBuffer>;
private:
    Buffer buffer_;
public:
    // 以性能为代价，直接用模板和 to_string 偷懒
    template<typename T> LogStream& operator<<(T); 
    void append(const char* buf, size_t len) { buffer_.append(buf, len); }
    void resetBuffer() { buffer_.reset(); }
    const Buffer& get_buffer() const { return buffer_; } // only for debug
};



} // namespace TCB

#endif // TCB_BASE_LOGSTREAM_H