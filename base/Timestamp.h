#ifndef TCB_BASE_TIMESTAMP_H
#define TCB_BASE_TIMESTAMP_H

#include <sys/time.h> // gettimeofday()
// #include <ctime> // c++11
// #include <chrono> // c++20 可以用 std::chrono，更方便
#include <string>
#include <cstring>

namespace TCB {

// Time stamp in UTC, in microseconds resolution.
//
// Usage: 
//
// TCB::Timestamp ts = TCB::Timestamp::now();
// std::cout << ts.toString() << ts.toFormattedString();
class Timestamp
{
private:
    int64_t microSecondsSinceEpoch_;
public:
    static const int kMicroSecondsPerSecond = 1000 * 1000;

    Timestamp () : microSecondsSinceEpoch_(0) {}
    explicit Timestamp (int64_t microSecondsSinceEpoch) : microSecondsSinceEpoch_(microSecondsSinceEpoch) {}

    std::string toString() const;
    std::string toFormattedString(bool showMicroseconds = true) const;
    std::string toCompactFormattedString(bool showMicroseconds = false) const;
    inline bool valid() const { return microSecondsSinceEpoch_ > 0; }

    static Timestamp now(); // 方便不用实例化直接使用

    // only for internal usage
    inline int64_t microSecondsSinceEpoch() const { return microSecondsSinceEpoch_; }
    inline int64_t secondsSinceEpoch() const { return microSecondsSinceEpoch_ / kMicroSecondsPerSecond;}
};

inline bool operator<(const Timestamp &lhs, const Timestamp &rhs) {
    return lhs.microSecondsSinceEpoch() < rhs.microSecondsSinceEpoch();
}
inline bool operator>(const Timestamp &lhs, const Timestamp &rhs) {
    return lhs.microSecondsSinceEpoch() > rhs.microSecondsSinceEpoch();
}
inline bool operator==(const Timestamp &lhs, const Timestamp &rhs) {
    return lhs.microSecondsSinceEpoch() == rhs.microSecondsSinceEpoch();
}
inline int64_t operator-(const Timestamp &lhs, const Timestamp &rhs) {
    return lhs.microSecondsSinceEpoch() - rhs.microSecondsSinceEpoch();
}
// end for Timestamp



} // namspace TCB

#endif // TCB_BASE_TIMESTAMP_H