#include "Timestamp.h"

namespace TCB
{

constexpr int time_buffer_size = 64;
thread_local char t_time[time_buffer_size];
thread_local char t_micro[time_buffer_size];
thread_local std::time_t t_lastSecond = 0;

// -------------- Timestamp Starts --------------

Timestamp::Timestamp() : microSecondsSinceEpoch_(0) {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    microSecondsSinceEpoch_ = tv.tv_sec * kMicroSecondsPerSecond + tv.tv_usec;
    // microSecondsSinceEpoch_ = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

std::string Timestamp::toString() const
{
  char buf[time_buffer_size] = {0};
  int seconds = secondsSinceEpoch();
  int microseconds = microSecondsSinceEpoch_ % kMicroSecondsPerSecond;
  snprintf(buf, time_buffer_size, "%d.%06d", seconds, microseconds);
  return buf;
}
std::string Timestamp::toFormattedString(bool showMicroseconds) const
{
    std::time_t seconds = secondsSinceEpoch();

    if (seconds != t_lastSecond) {
        t_lastSecond = seconds;
        struct tm tm_time;
        gmtime_r(&seconds, &tm_time);
        snprintf(t_time, time_buffer_size, "%4d%02d%02d %02d:%02d:%02d",
                        tm_time.tm_year + 1900, tm_time.tm_mon + 1, tm_time.tm_mday,
                        tm_time.tm_hour, tm_time.tm_min, tm_time.tm_sec);
    }

    if (showMicroseconds) {
        snprintf(t_micro, time_buffer_size, ".%6ld", microSecondsSinceEpoch_ % kMicroSecondsPerSecond);
    }

    return (showMicroseconds) ? std::string(t_time) + std::string(t_micro) : std::string(t_time);
}
// -------------- Timestamp Ends --------------

} // namespace TCB