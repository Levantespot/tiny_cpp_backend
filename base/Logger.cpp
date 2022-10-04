#include "Logger.h"

namespace TCB
{

// -------------- Logger::SourceFile Starts --------------
Logger::SourceFile::SourceFile(const char* filename) : data_(filename) {
    const char* slash = strrchr(filename, '/');
    if (slash)
    {
        data_ = slash + 1;
    }
    size_ = strlen(data_);
}

LogStream& operator<<(LogStream& s, const Logger::SourceFile& v) {
    s.append(v.get_data(), v.size());
    return s;
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
void defaultOutput(const char* msg, size_t len)
{
    fwrite(msg, 1, len, stdout);
}
void defaultFlush()
{
    fflush(stdout);
}
Logger::OutputFunc Logger::g_output = defaultOutput;    // 静态成员函数，需要在类外初始化
Logger::FlushFunc Logger::g_flush = defaultFlush;       

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

Logger::LogLevel Logger::g_logLevel = Logger::LogLevel::INFO;
Logger::LogLevel Logger::logLevel() { return g_logLevel; }
void Logger::set_logLevel(Logger::LogLevel level) { g_logLevel = level; }

// -------------- Logger Ends --------------

} // namespace TCB
