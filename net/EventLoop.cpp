#include "EventLoop.h"
#include "../base/Logger.h"
#include "Poller.h"

namespace TCB
{
namespace net
{

thread_local EventLoop* t_loopInThisThread = nullptr;

EventLoop::EventLoop()
  : looping_(false),
    threadId_(CurrentThread::tid()),
    activeChannels_(),
    currentActiveChannel_(nullptr),
    poller_(Poller::newDefaultPoller(this))
{
    LOG_DEBUG << "EventLoop created " << this << " in thread " << threadId_;
    if (t_loopInThisThread) { // If an event loop already exists, then abort.
        LOG_FATAL << "Another EventLoop " << t_loopInThisThread
                  << " exists in this thread " << threadId_;
    } else {
        t_loopInThisThread = this;
    }
}

EventLoop::~EventLoop() {
    LOG_DEBUG << "EventLoop " << this << " of thread " << threadId_
              << " destructs in thread " << CurrentThread::tid();
    t_loopInThisThread = nullptr;
}

void EventLoop::loop() {
    if (looping_) {
        LOG_ERROR << "Failed to loop since EventLoop " << this 
                  << " in thread " << threadId_ << " is already looping!";
    } else {
        looping_ = true;
        LOG_TRACE << "EventLoop " << this << " starts looping";

        // do some work
        LOG_INFO << "looping~" ; // test for test_EventLoop.cpp

        LOG_TRACE << "EventLoop " << this << " stops looping";
        looping_ = false;
    }
}

EventLoop* EventLoop::getEventLoopOfCurrentThread() {
    return t_loopInThisThread;
}

void EventLoop::assertInLoopThread() const {
    if (!isInLoopThread()){
        LOG_FATAL << "EventLoop::assertInLoopThread - EventLoop " 
            << this << " was created in threadId_ = " << threadId_
            << ", current thread id = " <<  CurrentThread::tid();
    }
}

void EventLoop::updateChannel(Channel* channel) {
    assertInLoopThread();
    poller_->updateChannel(channel);
}
void EventLoop::removeChannel(Channel* channel){
    assertInLoopThread();
    poller_->removeChannel(channel);
}
bool EventLoop::hasChannel(Channel* channel){
    assertInLoopThread();
    return poller_->hasChannel(channel);
}

} // namespace net
} // namespace TCB
