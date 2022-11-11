#ifndef TCB_BASE_ASYNCLOGGING_H
#define TCB_BASE_ASYNCLOGGING_H

#include <atomic>
#include <chrono>
#include <condition_variable>
#include <functional>  // bind
#include <memory>      // unique_lock, unique_ptr
#include <mutex>
#include <string>
#include <thread>
#include <vector>

#include "base/CountDownLatch.h"
#include "base/LogStream.h"

namespace TCB {

class LogFile;

class AsyncLogging : noncopyable {
 public:
  explicit AsyncLogging(const std::string& basename,
                        std::size_t rollSize = 2 * kLargeBuffer,  // 大于 kLargeBuffer 才有意义，如
                        int flushInterval = 3);
  ~AsyncLogging();
  void append(const char* logline, std::size_t len);
  void flush();

 private:
  void threadFunc();

  using Buffer = FixedBuffer<kLargeBuffer>;
  using BufferVector = std::vector<std::unique_ptr<Buffer>>;
  using BufferPtr = std::unique_ptr<Buffer>;

  const std::string basename_;                // 日志文件名
  const std::size_t rollSize_;                // 日志大小的滚动阈值
  const std::chrono::seconds flushInterval_;  // 刷缓存的最小间隔（秒）

  std::atomic<bool> running_;
  std::mutex mutex_;                   // 用于互斥修改成员变量
  CountDownLatch latch_;               // 控制子线程在父线程完全初始化后才运行
  std::unique_lock<std::mutex> lock_;  // 用于条件变量的同步
  std::condition_variable cond_;       // 用于同步
  BufferPtr currentBuffer_;            // 当前使用的缓存
  BufferPtr nextBuffer_;               // 备用缓存
  BufferVector buffers_;               // 待写入硬盘的已完成的缓存队列
  std::unique_ptr<LogFile> output;
  std::thread thread_;
};  // class AsyncLogging

}  // namespace TCB

#endif  // #ifndef TCB_BASE_ASYNCLOGGING_H