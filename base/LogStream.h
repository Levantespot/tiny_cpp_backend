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
    // FixedBuffer(const std::string &buf) : cur_(data_) {
    //     int size = buf.size();
    //     if (size <= SIZE) {
    //         memcpy(cur_, buf.c_str(), size);
    //         cur_ += size;
    //     } else {
    //         ignore(size);
    //     }
    // }
    // FixedBuffer(const std::stringstream &ss) : cur_(data_) {
    //     std::string buf = ss.str();
    //     int size = buf.size();
    //     if (size < SIZE) {
    //         memcpy(cur_, buf.c_str(), size);
    //         cur_ += size;
    //     } else {
    //         std::cerr << "Logs with length of " 
    //             << size << " was truncate!\n";
    //         memcpy(cur_, buf.c_str() + (size-SIZE), SIZE);
    //         cur_ += SIZE;
    //     }
    // }
    // 返回缓存的头指针
    inline const char* get_data() const { return data_; }
    // 写缓存
    void append(const char* buf, size_t len) {
        if (avail() > len) {
            memcpy(cur_, buf, len);
            cur_ += len;
        } else {
            ignore(len);
        }
    }
    // 返回缓存的大小
    inline size_t capacity() const { return SIZE; }
    // 返回缓存使用量
    inline size_t size() const { return static_cast<size_t>(cur_-data_); }
    // 返回缓存剩余量
    inline size_t avail() const { return SIZE - size(); }
    inline void bzero() { memset(data_, 0, sizeof(data_)); }
    // 重置
    inline void reset() { cur_ = data_; }
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

    inline void append(const char* buf, size_t len) { buffer_.append(buf, len); }
    inline void resetBuffer() { buffer_.reset(); }
    inline const Buffer& get_buffer() const { return buffer_; } // only for debug

    // 负责其他类型（如整数）的写入
    template<typename T> LogStream& operator<<(const T& v) {
        strm_ << v;
        const std::string &s(strm_.str());
        buffer_.append(s.c_str(), s.size());
        return *this;
    }
    LogStream& operator<<(char);
    // 负责 c 风格字符串指针的写入
    LogStream& operator<<(const char*);
    // 负责字符串的写入
    LogStream& operator<<(const std::string&);
    // 负责 Buffer 的写入
    LogStream& operator<<(const Buffer&);
private:
    std::stringstream strm_;
    Buffer buffer_;
    static const int kMaxNumericSize = 48;
};

} // namespace TCB

#endif // TCB_BASE_LOGSTREAM_H