#ifndef TCB_BASE_LOGSTREAM_H
#define TCB_BASE_LOGSTREAM_H

#include "noncopyable.h"
#include <iostream>
#include <string>
#include <sstream> // stringstream
#include <cstring> // memcpy

namespace TCB
{

const std::size_t kSmallBuffer = 4000;
const std::size_t kLargeBuffer = kSmallBuffer*1000;

template<std::size_t SIZE>
class FixedBuffer : noncopyable
{
private:
    char data_[SIZE];
    char* cur_;
public:
    FixedBuffer() : cur_(data_) { bzero(); }
    // 返回缓存的头指针
    inline const char* get_data() const { return data_; }
    // 写缓存
    void append(const char* buf, std::size_t len) {
        if (avail() > len) {
            memcpy(cur_, buf, len);
            cur_ += len;
        } else {
            ignore(len);
        }
    }
    // 返回缓存的大小
    inline std::size_t capacity() const { return SIZE; }
    // 返回缓存使用量
    inline std::size_t size() const { return static_cast<std::size_t>(cur_-data_); }
    // 返回缓存剩余量
    inline std::size_t avail() const { return SIZE - size(); }
    inline void bzero() { memset(data_, 0, sizeof(data_)); }
    // 重置
    inline void reset() { cur_ = data_; }
    //
    inline char* get_cur() { return cur_; }
    inline void add(std::size_t len) { cur_ += len; }
    // 忽略并输出错误信息
    inline void ignore(int len) const {
        std::cerr << "A log with length of " << len 
                << " was discarded since there is "
                << avail() << " space left in FixedBuffer!\n";
    }
}; // class FixedBuffer

class LogStream : noncopyable
{
public:
    using Buffer = FixedBuffer<kSmallBuffer>;

    LogStream() : buffer_() {};

    inline void append(const char* buf, std::size_t len) { buffer_.append(buf, len); }
    inline void resetBuffer() { buffer_.reset(); }
    inline const Buffer& get_buffer() const { return buffer_; } // only for debug

    LogStream& operator<<(bool v);
    LogStream& operator<<(short);
    LogStream& operator<<(unsigned short);
    LogStream& operator<<(int);
    LogStream& operator<<(unsigned int);
    LogStream& operator<<(long);
    LogStream& operator<<(unsigned long);
    LogStream& operator<<(long long);
    LogStream& operator<<(unsigned long long);
    LogStream& operator<<(const void*);
    LogStream& operator<<(float v);
    LogStream& operator<<(double);

    LogStream& operator<<(char);
    // 负责 c 风格字符串指针的写入
    LogStream& operator<<(const char*);
    // 负责字符串的写入
    LogStream& operator<<(const std::string&);
    // 负责 Buffer 的写入
    LogStream& operator<<(const Buffer&);
private:
    template<typename T>
    void formatInteger(T v);

    Buffer buffer_;
    static constexpr int kMaxNumericSize = 48;
};

} // namespace TCB

#endif // TCB_BASE_LOGSTREAM_H