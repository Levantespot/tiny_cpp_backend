# tiny_cpp_backend

一个 C++ 的练手项目，目标是实现一个多线程的服务器网络库后端。部分代码参考 [muduo](https://github.com/chenshuo/muduo)，并修改为 C++11 风格。

TODO 
- 多线程异步日志库
  - 前端
    - [x] FixedBuffer: 提供定长的缓存和相应的写入、重置等操作；
    - [x] LogStream: 包含一个小缓存，并重载 `<<` 操作符，将任意的输入转换为字符串然后写入该缓存；
    - [x] SourceFile: 编译时计算源文件的文件名；
    - [x] Timestamp: 提供格式化的精确到微秒的时间戳功能；
    - [x] Impl: 将事件、Level、文件名等信息同用户传来的信息组合在一起；
    - [x] Logger: 暴露给用户的 API；
  - 后端
    - [x] FileUtil: 封装 OS 提供的文件读写操作接口，并开启 I/O 缓冲，线程安全由上一层级调用者考虑；
    - [x] LogFile: 提供日志滚动功能，在 FileUtil 的基础上，保证按序写入硬盘；
    - [ ] AsyncLogging
  - 配置
    - 前端: Logger + 后端: LogFile = 单线程日志, 见 [示例文件](./base/test/test_Logger_with_LogFile.cpp).
    - 前端: Logger + 后端: AsyncLogging = 多线程异步日志，多线程主要指的是前端和后端为两个不同的线程。
- 网络库
