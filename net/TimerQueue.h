#ifndef TCB_NET_TIMERQUEUE_H
#define TCB_NET_TIMERQUEUE_H

#include <queue>

#include "../base/Timestamp.h"
#include "Channel.h"

namespace TCB {
namespace net {

class EventLoop;

class TimerQueue : noncopyable {
 public:
  explicit TimerQueue(EventLoop* loop);
  ~TimerQueue();
};

}  // namespace net
}  // namespace TCB

#endif