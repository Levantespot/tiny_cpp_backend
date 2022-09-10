# tiny_cpp_backend

一个 C++ 的练手项目，目标是实现一个多线程的服务器网络库后端。部分代码参考 [muduo](https://github.com/chenshuo/muduo)，并修改为 C++11 风格。

TODO 
- 多线程异步日志库
  - 前端
    - [x] FixedBuffer: 提供定长的缓存和相应的写入、重置等操作；
    - [x] LogStream: 重载 `<<` 操作符，将输入的字面量写入缓存；
    - [ ] Logger
    - [ ] SourceFile
  - 后端
- 网络库
