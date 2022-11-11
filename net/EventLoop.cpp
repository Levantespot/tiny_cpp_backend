#include "net/EventLoop.h"

#include <sys/eventfd.h>

#include "base/Logger.h"
#include "net/Channel.h"
#include "net/Poller.h"

namespace TCB {
namespace net {

thread_local EventLoop* t_loopInThisThread = nullptr;
const int kPollTimeMs = 10000;

int createEventfd() {
  int evtfd = ::eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC);
  if (evtfd < 0) {
    LOG_SYSERR << "Failed in eventfd";
    abort();
  }
  return evtfd;
}

EventLoop::EventLoop()
    : looping_(false),
      quit_(false),
      threadId_(CurrentThread::tid()),
      wakeupFd_(createEventfd()),
      wakeupChannel_(new Channel(this, wakeupFd_)),
      activeChannels_(),
      currentActiveChannel_(nullptr),
      poller_(Poller::newDefaultPoller(this)),
      mutex_(),
      pendingFunctors_(),
      callingPendingFunctors_(false) {
  LOG_DEBUG << "EventLoop created " << this << " in thread " << threadId_;
  if (t_loopInThisThread) {  // If an event loop already exists, then abort.
    LOG_FATAL << "Another EventLoop " << t_loopInThisThread << " exists in this thread "
              << threadId_;
  } else {
    t_loopInThisThread = this;
  }
  wakeupChannel_->setReadCallBack(std::bind(&EventLoop::clean_wakeup, this));
  wakeupChannel_->enableReading();
}

EventLoop::~EventLoop() {
  LOG_DEBUG << "EventLoop " << this << " of thread " << threadId_ << " destructs in thread "
            << CurrentThread::tid();
  t_loopInThisThread = nullptr;
  wakeupChannel_->disableAll();
  // wakeupChannel_->remove();
  ::close(wakeupFd_);
  t_loopInThisThread = nullptr;
}

void EventLoop::loop() {
  if (looping_) {
    LOG_ERROR << "Failed to loop since EventLoop " << this << " in thread " << threadId_
              << " is already looping!";
  } else {
    looping_ = true;
    quit_ = false;
    LOG_TRACE << "EventLoop " << this << " starts looping";

    while (!quit_) {
      activeChannels_.clear();
      poller_->poll(kPollTimeMs, activeChannels_);
      for (auto& it : activeChannels_) {
        it->handleEvent();
      }
      doPendingFunctors();
    }

    LOG_TRACE << "EventLoop " << this << " stops looping";
    looping_ = false;
  }
}

void EventLoop::quit() {
  quit_ = true;
  if (!isInLoopThread()) {
    wakeup();
  }
}

EventLoop* EventLoop::getEventLoopOfCurrentThread() { return t_loopInThisThread; }

void EventLoop::assertInLoopThread() const {
  if (!isInLoopThread()) {
    LOG_FATAL << "EventLoop::assertInLoopThread - EventLoop " << this
              << " was created in threadId_ = " << threadId_
              << ", current thread id = " << CurrentThread::tid();
  }
}

void EventLoop::wakeup() {
  uint64_t one = 1;
  ssize_t n = write(wakeupFd_, &one, sizeof one);
  if (n != sizeof one) {
    LOG_ERROR << "EventLoop::wakeup() writes " << n << " bytes instead of 8";
  }
}

void EventLoop::clean_wakeup() {
  uint64_t one = 1;
  ssize_t n = read(wakeupFd_, &one, sizeof one);
  if (n != sizeof one) {
    LOG_ERROR << "EventLoop::handleRead() reads " << n << " bytes instead of 8";
  }
}

void EventLoop::updateChannel(Channel* channel) {
  assertInLoopThread();
  poller_->updateChannel(channel);
}
void EventLoop::removeChannel(Channel* channel) {
  assertInLoopThread();
  poller_->removeChannel(channel);
}
bool EventLoop::hasChannel(Channel* channel) {
  assertInLoopThread();
  return poller_->hasChannel(channel);
}

void EventLoop::runInLoop(Functor cb) {
  if (isInLoopThread()) {
    cb();
  } else {
    queueInLoop(std::move(cb));
  }
}
void EventLoop::queueInLoop(Functor cb) {
  {
    std::lock_guard<std::mutex> lk(mutex_);
    pendingFunctors_.push_back(std::move(cb));
  }

  if (!isInLoopThread() || callingPendingFunctors_) {
    // 进入该函数体有以下两种情况：
    // 1. 若不在 loop 线程中
    // 2. 在 loop 线程中，则一定是在 doPendingFunctors() 中调用 queueInLoop()
    // 都必须唤醒，以保证新加的 callback 被及时调用
    wakeup();
  }
}

void EventLoop::doPendingFunctors() {
  std::vector<Functor> functors;
  callingPendingFunctors_ = true;
  {
    std::lock_guard<std::mutex> lk(mutex_);
    functors.swap(pendingFunctors_);
  }
  for (auto& func : functors) {
    func();
  }
  callingPendingFunctors_ = false;
}

}  // namespace net
}  // namespace TCB
