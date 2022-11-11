#ifndef TCB_NET_EVENTLOOPTHREAD_H
#define TCB_NET_EVENTLOOPTHREAD_H

#include <condition_variable>
#include <functional>
#include <mutex>
#include <thread>

#include "base/noncopyable.h"

namespace TCB {
namespace net {

class EventLoop;

class EventLoopThread : noncopyable {
 public:
  using ThreadInitCallback = std::function<void(EventLoop*)>;

  EventLoopThread(const ThreadInitCallback& cb = ThreadInitCallback());
  ~EventLoopThread();

  EventLoop* startLoop();

 private:
  void threadFunc();

  bool started;
  EventLoop* loop_;
  std::mutex mutex_;
  std::condition_variable cond_;
  ThreadInitCallback callback_;
  std::unique_ptr<std::thread> ptrThread_;
};

}  // namespace net
}  // namespace TCB

#endif  // TCB_NET_EVENTLOOPTHREAD_H