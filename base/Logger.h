#ifndef TCB_BASE_LOGGING_H
#define TCB_BASE_LOGGING_H

#include "LogStream.h"
#include "Timestamp.h"
#include <thread> // this_thread::get_id
#include <cstring> // strerror
#include <cstdlib> // abort
#include <functional>
namespace TCB
{

class Logger
{   
/* ------- 日志等级 -------*/
public:
    enum LogLevel {
        TRACE,
        DEBUG,
        INFO,
        WARN,
        ERROR,
        FATAL,
        NUM_LOG_LEVELS,
    };
    // 返回日志等级
    static LogLevel logLevel();
    // 设置日志等级
    static void set_logLevel(LogLevel level);
private:
    static LogLevel g_logLevel;

/* ------- 辅助类 -------*/
public:
    // 构造时输入文件路径，得到文件名
    // 结合宏，可以在编译时就获得调用日志库的文件的文件名
    class SourceFile
    {
    private:
        const char* data_;
        size_t size_;
    public:
        // template<size_t N> SourceFile(const char (&arr)[N]) : data_(arr), size_(N-1) {
        //     const char* slash = strrchr(data_, '/');
        //     if (slash)
        //     {
        //         data_ = slash + 1;
        //         size_ -= data_ - arr;
        //     }
        // }
        SourceFile(const char* filename);
        SourceFile(const SourceFile &) = default;
        
        const char* get_data() const { return data_; }
        const size_t size() const { return size_; }
    }; // class Source File
private:
    // 封装格式化日志信息的类
    class Impl
    {
    public:
        Impl() = delete;
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
public:
    // Internal usage only!
    LogStream& stream() { return impl_.stream_; }

/* ------- 构造析构 -------*/
public:
    Logger(SourceFile file, int line);
    Logger(SourceFile file, int line, LogLevel level);
    Logger(SourceFile file, int line, LogLevel level, const char* func);
    Logger(SourceFile file, int line, bool toAbort);
    // 析构的时候负责把所有信息按格式输出
    ~Logger();

/* ------- 输出 & 刷缓存 -------*/
public:
    // 输出函数的函数类型
    // void test(const char* msg, size_t len);
    // typedef std::function<void(const char* msg, size_t len)> OutputFunc1;
    // static void setOutput(OutputFunc1);
    // B b;
    // setOutput(std::bind(&B::test,&b));

    using OutputFunc = std::function<void (const char* msg, size_t len)>;
    // 刷缓存函数的函数类型
    using FlushFunc = std::function<void ()>;
    // 设置输出函数的入口，设置为静态成员方便共享
    static void setOutput(OutputFunc);
    // 设置刷缓存函数的入口，设置为静态成员方便共享
    static void setFlush(FlushFunc);
private:
    static OutputFunc g_output;
    static FlushFunc g_flush;
    
}; // class Logger

// 给 LogStream 增加一个写入 SourceFile 类型的操作
LogStream& operator<<(LogStream& s, const Logger::SourceFile& v);

// 定义一系列宏，方便直接调用
// LOG_TRACE, LOG_DEBUG, LOG_INFO 受日志等级控制，低于日志等级的日志会被忽略

// 开发日志
#define LOG_TRACE if (TCB::Logger::logLevel() <= TCB::Logger::TRACE) \
    TCB::Logger(__FILE__, __LINE__, TCB::Logger::TRACE, __func__).stream()
// 调试日志
#define LOG_DEBUG if (TCB::Logger::logLevel() <= TCB::Logger::DEBUG) \
    TCB::Logger(__FILE__, __LINE__, TCB::Logger::DEBUG, __func__).stream()
// 运行日志
#define LOG_INFO if (TCB::Logger::logLevel() <= TCB::Logger::INFO) \
    TCB::Logger(__FILE__, __LINE__).stream()
// 警告日志
#define LOG_WARN TCB::Logger(__FILE__, __LINE__, TCB::Logger::WARN).stream()
// 错误日志（继续运行）
#define LOG_ERROR TCB::Logger(__FILE__, __LINE__, TCB::Logger::ERROR).stream()
// 错误日志（强行退出）
#define LOG_FATAL TCB::Logger(__FILE__, __LINE__, TCB::Logger::FATAL).stream()
// 错误日志，并输出 错误日志（继续运行）
#define LOG_SYSERR TCB::Logger(__FILE__, __LINE__, false).stream()
// 错误日志，并输出 errono（强行退出）
#define LOG_SYSFATAL TCB::Logger(__FILE__, __LINE__, true).stream()

} // namespace TCB

#endif // TCB_BASE_LOGGING_H