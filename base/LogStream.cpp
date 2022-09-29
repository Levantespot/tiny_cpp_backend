#include "LogStream.h"
#include <iostream>

using namespace TCB;

// -------------- FixedBuffer Starts --------------

template<size_t SIZE>
void FixedBuffer<SIZE>::append(const char* buf, size_t len) {
    if (FixedBuffer<SIZE>::avail() > len) {
        memcpy(cur_, buf, len);
        cur_ += len;
    } else {
        std::cerr << "TCB::FixedBuffer<SIZE>::append: A log was "
            "discarded since there is out of space in FixedBuffer!\n";
    }
}
// -------------- FixedBuffer Ends --------------

// -------------- LogStream Starts --------------

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