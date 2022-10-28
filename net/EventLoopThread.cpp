#include "EventLoopThread.h"
#include "EventLoop.h"

#include "../base/Logger.h"


namespace TCB
{
namespace net
{
    
EventLoopThread::EventLoopThread(const ThreadInitCallback& cb) 
  : started(false),
    loop_(nullptr),
    mutex_loop_(),
    mutex_cond_(),
    cond_(),
    callback_(cb),
    ptrThread_(nullptr)
{
    // empty
}

EventLoopThread::~EventLoopThread() {
    if (loop_ != nullptr) {
        loop_->quit();
        if (started) {
            ptrThread_->join();
        }
    }
}

EventLoop* EventLoopThread::startLoop() {
    EventLoop* ptr_loop(nullptr);

    if (!started) {
        started = true;
        ptrThread_.reset(new std::thread(
            std::bind(&EventLoopThread::threadFunc, this)));

        {
            // std::unique_lock<std::mutex> ulk(mutex_cond_);
            std::unique_lock<std::mutex> ulk(mutex_loop_);
            cond_.wait(ulk, [this] () { return loop_ != nullptr; });
            ptr_loop = loop_;
        }        

    } else {
        LOG_ERROR << "EventLoopThread::startLoop() already started!";
    }

    return ptr_loop;
}

void EventLoopThread::threadFunc() {
    EventLoop loop;

    if (callback_) {
        callback_(&loop);
    }

    {
        std::lock_guard<std::mutex> lc(mutex_loop_);
        loop_ = &loop;
        cond_.notify_one();
    }

    loop.loop();

    {
        std::lock_guard<std::mutex> lk(mutex_loop_);
        loop_ = nullptr;
    }
}



} // namespace net
} // namespace TCB


