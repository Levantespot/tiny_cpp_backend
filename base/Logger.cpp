#include "Logger.h"

namespace TCB
{

// -------------- Logger::SourceFile Starts --------------

template<size_t N>
Logger::SourceFile::SourceFile(const char (&arr)[N]) : data_(arr), size_(N-1) {
    const char* slash = strrchr(data_, '/');
    if (slash)
    {
        data_ = slash + 1;
        size_ -= data_ - arr;
    }
}

Logger::SourceFile::SourceFile(const char* filename) : data_(filename) {
    const char* slash = strrchr(filename, '/');
    if (slash)
    {
        data_ = slash + 1;
    }
    size_ = strlen(data_);
}
// -------------- Logger::SourceFile Ends --------------

// -------------- Logger::Impl Starts --------------
const char* LogLevelName[Logger::NUM_LOG_LEVELS] =
{
    "TRACE ",
    "DEBUG ",
    "INFO  ",
    "WARN  ",
    "ERROR ",
    "FATAL ",
};
Logger::Impl::Impl(LogLevel level, int savedErrno, const SourceFile& file, int line) 
  : time_(Timestamp::now()),
    stream_(),
    level_(level),
    line_(line),
    basename_(file)
{
    stream_ << time_.toFormattedString() << " "; // 写时间
    stream_ << std::this_thread::get_id() << " "; // 写当前线程的id
    stream_ << LogLevelName[level] << " "; // 写日志等级
    if (savedErrno != 0) { // 写错误信息
        stream_ << std::strerror(savedErrno) << " (errno=" << savedErrno << ") ";
    }
    
}
void Logger::Impl::finish() {
    stream_ << " - " << basename_ << ':' << line_ << '\n';
}
// -------------- Logger::Impl Ends --------------


// -------------- Logger Starts --------------
void defaultOutput(const char* msg, int len)
{
  size_t n = fwrite(msg, 1, len, stdout);
}
void defaultFlush()
{
  fflush(stdout);
}
Logger::OutputFunc g_output = defaultOutput;    // 由于非单例模式，因此需要设置为非成员变量
Logger::FlushFunc g_flush = defaultFlush;       // 只需要修改一次后续的写日志也会生效

Logger::Logger(SourceFile file, int line)
  : impl_(Logger::INFO, 0, file, line) {}

Logger::Logger(SourceFile file, int line, LogLevel level)
  : impl_(level, 0, file, line) {}

Logger::Logger(SourceFile file, int line, LogLevel level, const char* func)
  : impl_(level, 0, file, line) { impl_.stream_ << func << ' '; }

Logger::Logger(SourceFile file, int line, bool toAbort) 
  : impl_(toAbort?FATAL:ERROR, errno, file, line) {}

Logger::~Logger() {
    impl_.finish();
    const LogStream::Buffer& buf(stream().get_buffer());
    g_output(buf.get_data(), buf.size());
    if (impl_.level_ == FATAL)
    {
        g_flush();
        abort();
    }
}

void Logger::setOutput(OutputFunc out) {
    g_output = out;
}
void Logger::setFlush(FlushFunc flush) {
    g_flush = flush;
}

Logger::LogLevel g_logLevel = Logger::LogLevel::INFO;
Logger::LogLevel Logger::logLevel() { return g_logLevel; }
void Logger::set_logLevel(Logger::LogLevel level) { g_logLevel = level; }

// -------------- Logger Ends --------------

} // namespace TCB
