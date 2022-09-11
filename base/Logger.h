#ifndef TCB_BASE_LOGGING_H
#define TCB_BASE_LOGGING_H

#include "LogStream.h"
#include "Timestamp.h"
#include <thread> // this_thread::get_id
#include <cstring> // strerror
#include <cstdlib> // abort

namespace TCB
{

class Logger
{   
public:
    enum LogLevel
    {
        TRACE,
        DEBUG,
        INFO,
        WARN,
        ERROR,
        FATAL,
        NUM_LOG_LEVELS,
    };

    // compile time calculation of basename of source file
    class SourceFile
    {
    private:
        const char* data_;
        size_t size_;
    public:
        template<size_t N> SourceFile(const char (&arr)[N]);
        explicit SourceFile(const char* filename);
        SourceFile(const SourceFile &) = default;
        
        const char* get_data() const { return data_; }
        const size_t size() const { return size_; }
    }; // class Source File

    Logger(SourceFile file, int line);
    Logger(SourceFile file, int line, LogLevel level);
    Logger(SourceFile file, int line, LogLevel level, const char* func);
    Logger(SourceFile file, int line, bool toAbort);
    // 析构的时候负责把所有信息按格式输出
    ~Logger();

    // Internal usage only!
    LogStream& stream() { return impl_.stream_; }

    // 输出函数的函数类型
    using OutputFunc = void (*)(const char* msg, int len);
    // 刷缓存函数的函数类型
    using FlushFunc = void (*)();
    // 设置输出函数的入口
    static void setOutput(OutputFunc);
    // 设置刷缓存函数的入口
    static void setFlush(FlushFunc);
    // static void setTimeZone(const TimeZone& tz);

private:
    class Impl
    {
    public:
        // Impl() = delete;
        // 构造的同时，写入事件、线程id、日志级别，错误信息（如果有的话）。
        Impl(LogLevel level, int savedErrno, const SourceFile& file, int line);
        // 最后写入文件名和行号
        void finish(); 

        Timestamp time_;
        LogStream stream_;
        LogLevel level_;
        int line_;
        SourceFile basename_;
    };

    Impl impl_;
    
}; // class Logger

// 给 LogStream 增加一个写入 SourceFile 类型的操作
LogStream& operator<<(LogStream& s, const Logger::SourceFile& v) {
    s.append(v.get_data(), v.size());
    return s;
}
// 定义一系列宏，方便直接调用
#define LOG_TRACE TCB::Logger(__FILE__, __LINE__, TCB::Logger::TRACE, __func__).stream()
#define LOG_DEBUG TCB::Logger(__FILE__, __LINE__, TCB::Logger::DEBUG, __func__).stream()
#define LOG_INFO TCB::Logger(__FILE__, __LINE__).stream()
#define LOG_WARN TCB::Logger(__FILE__, __LINE__, TCB::Logger::WARN).stream()
#define LOG_ERROR TCB::Logger(__FILE__, __LINE__, TCB::Logger::ERROR).stream()
#define LOG_FATAL TCB::Logger(__FILE__, __LINE__, TCB::Logger::FATAL).stream()
#define LOG_SYSERR TCB::Logger(__FILE__, __LINE__, false).stream()
#define LOG_SYSFATAL TCB::Logger(__FILE__, __LINE__, true).stream()

} // namespace TCB

#endif // TCB_BASE_LOGGING_H