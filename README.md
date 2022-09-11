# tiny_cpp_backend

一个 C++ 的练手项目，目标是实现一个多线程的服务器网络库后端。部分代码参考 [muduo](https://github.com/chenshuo/muduo)，并修改为 C++11 风格。

TODO 
- 多线程异步日志库
  - 前端
    - [x] FixedBuffer: 提供定长的缓存和相应的写入、重置等操作；
    - [x] LogStream: 重载 `<<` 操作符，将任意的输入转换为字符串然后写入缓存；
    - [x] SourceFile: 编译时计算源文件的文件名；
    - [x] Timestamp: 提供格式化的精确到微秒的时间戳功能；
    - [x] Impl: 将事件、Level、文件名等信息同用户传来的信息组合在一起；
    - [x] Logger: 暴露给用户的 API；
  - 后端
    - [ ] PASS;
- 网络库
