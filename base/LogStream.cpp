#include "LogStream.h"

namespace TCB {

// -------------- LogStream Starts --------------
LogStream& LogStream::operator<<(char v) { 
    buffer_.append(&v, 1);
    return *this;
}
LogStream& LogStream::operator<<(const char* str) {
    if (str) {
        buffer_.append(str, strlen(str));
    } else {
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

}