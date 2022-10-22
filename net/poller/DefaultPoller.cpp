#include "PollPoller.h"
#include "../Poller.h"

using namespace TCB::net;

Poller* Poller::newDefaultPoller(EventLoop* loop) {
    return new PollPoller(loop);
}