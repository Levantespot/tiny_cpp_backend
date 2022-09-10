#include "LogStream.h"

using namespace TCB;

// -------------- FixedBuffer Start --------------
template<size_t SIZE>
void FixedBuffer<SIZE>::append(const char* buf, size_t len) {
    if (FixedBuffer<SIZE>::avail() > len) {
        memcpy(cur_, buf, len);
        cur_ += len;
    }
}
// -------------- FixedBuffer End --------------

// -------------- LogStream Start --------------
template<typename T>
LogStream& LogStream::operator<<(T v) {
    std::string s = to_string(v);
    append(s.c_str(), s.size());
    return *this;
}

// -------------- LogStream End --------------