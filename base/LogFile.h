#ifndef TCB_BASE_LOGFILE_H
#define TCB_BASE_LOGFILE_H

#include "noncopyable.h"
#include "FileUtil.h"
#include "Timestamp.h"
#include <mutex>
#include <memory> // unique_ptr
#include <string>
#include <ctime> // std::time_t

namespace TCB
{   

// 封装日志的自动刷新、文件滚动的功能
class LogFile : noncopyable
{
public:
    LogFile(const std::string& basename,
            std::size_t rollSize,
            bool threadSafe = true,
            int flushInterval = 3,
            int checkEveryN = 1024);
    ~LogFile() = default;
    void append(const char* logline, std::size_t len);
    void flush();   // 刷缓存
    bool rollFile(); // 滚动日志文件

private:
    void append_unlocked(const char* logline, std::size_t len);

    static std::string createLogFileName(const std::string& basename, std::time_t now);

    const std::string basename_; // 日志文件名
    const std::size_t rollSize_; // 日志大小的滚动阈值
    const int flushInterval_; // 刷缓存的最小间隔（秒）
    const int checkEveryN_; // 检查是否刷缓存或者滚动日志的次数阈值

    int count_; // 当前的操作次数

    std::unique_ptr<std::mutex> mutex_;
    std::time_t startOfPeriod_; // 最近一次刷缓存的时间（天，从 0 开始计数）
    std::time_t lastRoll_; // 最近一次滚动日志的时间（秒）
    std::time_t lastFlush_; // 最近一次刷缓存的时间（秒）
    std::unique_ptr<FileUtil::AppendFile> file_;

    const static int kRollPerSeconds_ = 60*60*24;
};
    
} // namespace TCB



#endif // #ifndef TCB_BASE_LOGFILE_H