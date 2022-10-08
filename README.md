# tiny_cpp_backend

一个 C++ 的练手项目，目标是实现一个多线程的服务器网络库后端。部分代码参考 [muduo](https://github.com/chenshuo/muduo)，并修改为 C++11 风格。

- 多线程异步日志库
  - 前端
    - [x] FixedBuffer: 提供定长的缓存和相应的写入、重置等操作；
    - [x] LogStream: 包含一个小缓存，并重载 `<<` 操作符，将任意的输入转换为字符串然后写入该缓存；
    - [x] SourceFile: 编译时计算源文件的文件名；
    - [x] Timestamp: 提供格式化的精确到微秒的时间戳功能；
    - [x] Impl: 将事件, Level, 文件名等信息同用户传来的信息组合在一起；
    - [x] Logger: 暴露给用户的 API，封装了上述类；
  - 后端
    - [x] FileUtil: 封装 OS 提供的文件读写操作接口，并开启 I/O 缓冲，线程安全由上一层级调用者考虑；
    - [x] LogFile: 提供日志滚动功能，在 FileUtil 的基础上，保证按序写入硬盘；
    - [x] AsyncLogging: 创建一个子线程负责用 LogFile 写入硬盘，并通过大缓存队列来减少读写硬盘的次数，
  - 配置
    - 前端: Logger + 后端: LogFile = 单线程日志, 见 [示例](./base/test/test_Logger_with_LogFile.cpp).
    - 前端: Logger + 后端: AsyncLogging = 多线程异步日志, 见 [示例](./base/test/test_Logger_with_Async.cpp)，多线程主要指的是日志的前端和后端为两个不同的线程。
  - 优化
    - benchmark 见 [单线程日志](./base/test/bench_Logger_with_LogFile.cpp) 和 [异步日志](./base/test/bench_Logger_with_Async.cpp);
    - 尽可能地显示内联成员函数 `inline` & 开启优化等级 `-O1` or `-O2`
    - 用 `thread_local` 变量缓存每个线程的 id，每次写日志的时候就不必重复调用 `std::this_thread::get_id()` 了;
    - 将 Timestamp 中的日期（年/月/日-时/分/秒）缓存，在同一秒内，只更新微秒部分;
    - 在 `LogStream` 中, 用「模板 + `stringstream`」的方式写入整数及其他类型的输入;
    - 将 `Sourcefile` 的构造函数定义为模板，编译时就能计算部分结果;

- 网络库 TODO
