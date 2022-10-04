#ifndef TCB_BASE_LOGSTREAM_H
#define TCB_BASE_LOGSTREAM_H

#include "noncopyable.h"
#include <iostream>
#include <string>
#include <sstream> // stringstream
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
    FixedBuffer() : cur_(data_) { bzero(); }
    // 返回缓存的头指针
    const char* get_data() const { return data_; }
    // 写缓存
    void append(const char* buf, size_t len) {
        if (avail() > len) {
            memcpy(cur_, buf, len);
            cur_ += len;
        } else {
            std::cerr << "TCB::FixedBuffer<SIZE>::append: A log was "
                "discarded since there is out of space in FixedBuffer!\n";
        }
    }
    // 返回缓存的大小
    size_t capacity() const { return SIZE; }
    // 返回缓存使用量
    size_t size() const { return static_cast<size_t>(cur_-data_); }
    // 返回缓存剩余量
    size_t avail() const { return SIZE - size(); }
    void bzero() { memset(data_, 0, sizeof(data_)); }
    // 重置
    void reset() { cur_ = data_; }
}; // class FixedBuffer

class LogStream : noncopyable
{
public:
    using Buffer = FixedBuffer<kSmallBuffer>;

    void append(const char* buf, size_t len) { buffer_.append(buf, len); }
    void resetBuffer() { buffer_.reset(); }
    const Buffer& get_buffer() const { return buffer_; } // only for debug

    // 负责算术类型的写入
    template<typename T> LogStream& operator<<(T v) {
        std::stringstream strm;
        strm << v;
        const std::string &s(strm.str());
        buffer_.append(s.c_str(), s.size());
        return *this;
    }
    // 负责单个字符的写入
    LogStream& operator<<(char);
    // 负责 c 风格字符串指针的写入
    LogStream& operator<<(const char*);
    // 负责字符串的写入
    LogStream& operator<<(const std::string&);
    // 负责 Buffer 的写入
    LogStream& operator<<(const Buffer&);
private:
    Buffer buffer_;
    static const int kMaxNumericSize = 48;
};

} // namespace TCB

#endif // TCB_BASE_LOGSTREAM_H