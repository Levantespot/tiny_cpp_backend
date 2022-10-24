# tiny_cpp_backend

一个 C++ 的练手项目，目标是实现一个多线程的服务器网络库后端。部分代码参考 [muduo](https://github.com/chenshuo/muduo)，并修改为 C++11 风格。

多线程异步日志库
- 前端
  - [x] FixedBuffer: 提供定长的缓存和相应的写入、重置等操作；
  - [x] LogStream: 包含一个小缓存，并重载 `<<` 操作符，将任意的输入转换为字符串然后写入该缓存；
  - [x] SourceFile: 编译时计算源文件的文件名；
  - [x] Timestamp: 提供格式化的精确到微秒的时间戳功能；
  - [x] Impl: 将事件, Level, 文件名等信息同用户传来的信息组合在一起；
  - [x] Logger: 暴露给用户的 API，封装了上述类；
- 后端
  - [x] AppendFile: 封装 OS 提供的文件读写操作接口，并开启 I/O 缓冲，线程安全由上一层级调用者考虑；
  - [x] LogFile: 提供日志滚动功能，在 FileUtil 的基础上，保证按序写入硬盘；
  - [x] AsyncLogging: 创建一个子线程负责用 LogFile 写入硬盘，并通过大缓存队列来减少读写硬盘的次数，
- 配置
  - 前端: Logger + 后端: LogFile = 单线程日志, 见 [示例](./base/test/test_Logger_with_LogFile.cpp).
  - 前端: Logger + 后端: AsyncLogging = 多线程异步日志, 见 [示例](./base/test/test_Logger_with_Async.cpp)，多线程主要指的是日志的前端和后端为两个不同的线程。
- 优化
  - benchmark 见 [单线程日志](./base/test/bench_Logger_with_LogFile.cpp) 和 [异步日志](./base/test/bench_Logger_with_Async.cpp);
  - 尽可能地显示内联成员函数 `inline` & 开启优化等级 `-O1` or `-O2`
  - 用 `thread_local` 类型的变量缓存每个线程的 id，每次写日志的时候就不必重复调用 `this_thread::get_id()` 了;
  - 将 Timestamp 中的日期（年/月/日-时/分/秒）缓存，在同一秒内，只更新微秒部分;
  - 在 `LogStream` 中, 曾试过用「模板 + `stringstream`」将输入转换为字符串，但是会非常慢，推测是由于 `stringstream` 的实例化开销较大;
  - 将 `Sourcefile` 的构造函数定义为模板，编译时就能计算部分结果（提升较小）;
  - `AsyncLogging` 中的大缓存使用智能指针管理，需要写入磁盘时直接 `swap` 指针后直接交给 `LogFile`，尽可能地减少临界区的长度;

```mermaid
classDiagram

class FixedBuffer~size_t SIZE~{
    -data_[SIZE] : char
    -cur_ : char*
    +size() size_t
    +append(buf : const char* , len : size_t)
    +get_data() const char*
}

class LogStream{
    -buffer_ : FixedBuffer~kSmallBuffer~
    +append(buf : const char* , len : size_t)
    +operator<<() LogStream&
} 

class SourceFile~int N~{
    -data_ : const char*
    -size_ : size_t
    +SourceFile(arr : const char &[N])
    +get_data() : const char* 
    +size() : size_t
} 

class Timestamp{
    -microSecondsSinceEpoch_ : int64_t
    +toFormattedString() string
    +microSecondsSinceEpoch() int64_t
    +secondsSinceEpoch() int64_t
} 

class Impl{
    +time_ : Timestamp
    +stream_ : LogStream
    +level_ : LogLevel
    +line_ : int
    +basename_ : SourceFile
    +Impl(level : LogLevel , savedErrno : int , file : const SourceFile& , line : int )
    +finish()
} 

class Logger{
    -impl_ : Impl
    -static g_output : OutputFunc
    -static g_flush : FlushFunc
    Logger(file : SourceFile, line : int )
    Logger(file : SourceFile, line : int , level : LogLevel)
    Logger(file : SourceFile, line : int , level : LogLevel, func : const char*)
    Logger(file : SourceFile, line : int , toAbort : bool)
    ~Logger()
} 

Logger *-- Impl
Impl *-- Timestamp
Impl *-- SourceFile
Impl *-- LogStream
LogStream *-- FixedBuffer

class AppendFile{
    -fp_ : FILE* 
    -buffer_ : char[]
    -written_bytes_ : size_t
    +AppendFile(filename : const char* )
    +append(logline : const char* , len : size_t)
    +flush()
    +writenBytes () size_t
} 

class LogFile{
    -basename_ : const string
    -rollSize_ : const size_t
    -flushInterval_ : const int
    -checkEveryN_ : const int
    -count_ : int
    -mutex
    -file_ : unique_ptr<AppendFile>
    -append_unlocked(logline : const char* , len : size_t)
    -createLogFileName(logline : const char* , now : time_t) string
    +LogFile(basename : const std::string&, 
            rollSize : size_t,
            threadSafe : bool,
            flushInterval : int,
            checkEveryN : int)
    +append(logline : const char* , len : size_t)
    +flush()
    +rollFile() : bool
} 

class AsyncLogging{
    -basename_ : const string
    -rollSize_ : const size_t
    -flushInterval_ : const int
    -mutex : mutex
    -latch_ : CountDownLatch
    -lock_ : unique_lock<mutex>
    -cond_ : condition_variable
    -currentBuffer_ : unique_ptr<FixedBuffer<kLargeBuffer>>
    -nextBuffer_ : unique_ptr<FixedBuffer<kLargeBuffer>>
    -buffers_ : vector<unique_ptr<FixedBuffer<kLargeBuffer>>>
    -output : unique_ptr<LogFile> 
    -thread_ : thread
    +append(logline : const char* , len : size_t)
    +flush()
}

class CountDownLatch{
    - mutex_ : mutex
    - cond_ : condition_variable
    - count_ : int
    + CountDownLatch( count : int)
    + wait()
    + countDown()
}

Logger --> AsyncLogging
AsyncLogging *-- LogFile
AsyncLogging *-- CountDownLatch
AsyncLogging *-- FixedBuffer
LogFile *-- AppendFile
```

网络库 TODO
- [ ] Channel: 负责注册和响应 IO 事件（对 struct pollfd 和各类事件处理方法的封装），一个 Channel 与 EventLoop 一对一绑定，
- [ ] EventLoop: 线程事件循环器，负责 IO 和定时器任务的分派；
- [x] Poller: 负责 I/O 复用的抽象基类；
  - [x] PollPoller
  - [ ] EpollPoller

