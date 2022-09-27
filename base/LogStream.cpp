#include "LogStream.h"
// #include <typeinfo> // typeid

using namespace TCB;

// -------------- FixedBuffer Starts --------------

template<size_t SIZE>
void FixedBuffer<SIZE>::append(const char* buf, size_t len) {
    if (FixedBuffer<SIZE>::avail() > len) {
        memcpy(cur_, buf, len);
        cur_ += len;
    } else {
        throw "TCB::FixedBuffer<SIZE>::append: out of space to append!";
    }
}
// -------------- FixedBuffer Ends --------------

// -------------- LogStream Starts --------------

// template<typename T> LogStream& LogStream::operator<<(T v) {
//     std::stringstream strm;
//     strm << v;
//     std::string s(strm.str());
//     buffer_.append(s.c_str(), s.size());
//     return *this;
// }
LogStream& LogStream::operator<<(char v) { 
    buffer_.append(&v, 1);
    return *this;
}
LogStream& LogStream::operator<<(const char* str) {
    if (str) {
        buffer_.append(str, strlen(str));
    }
    else {
        buffer_.append("(null)", 6);
    }
    return *this;
}
LogStream& LogStream::operator<<(const std::string& v) {
    buffer_.append(v.c_str(), v.size());
    return *this;
}
LogStream& LogStream::operator<<(const Buffer& v) {
    buffer_.append(v.get_data(), v.size());
    return *this;
}
// -------------- LogStream Ends --------------