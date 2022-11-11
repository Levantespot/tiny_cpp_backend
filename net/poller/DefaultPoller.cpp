#include "net/Poller.h"
#include "net/poller/PollPoller.h"

using namespace TCB::net;

Poller* Poller::newDefaultPoller(EventLoop* loop) { return new PollPoller(loop); }