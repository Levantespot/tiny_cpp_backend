#ifndef TCB_NET_EVENTLOOP_H
#define TCB_NET_EVENTLOOP_H

#include <vector>
#include <functional>
#include <atomic>
#include <memory>
#include <mutex>

#include "../base/CurrentThread.h"
#include "../base/Timestamp.h"
#include "../base/noncopyable.h"

namespace TCB
{
namespace net
{

class Channel;
class Poller;

// 事件循环器，负责 IO 和定时器任务的分派
// 一个线程中只能存在一个 EventLoop
class EventLoop : noncopyable
{
public:
    using Functor = std::function<void ()>;

    EventLoop();
    ~EventLoop();

    void loop(); // 在循环中不断处理业务 TODO
    void quit(); // 通知停止循环
    static EventLoop* getEventLoopOfCurrentThread();

    inline bool isInLoopThread() const { return threadId_ == CurrentThread::tid(); }
    void assertInLoopThread() const; // 确保当前线程是 EventLoop 的线程

    /* 更新、移除、判断 poller 中的 map<int, channel> */
    void updateChannel(Channel* channel);
    void removeChannel(Channel* channel);
    bool hasChannel(Channel* channel);

    void wakeup(); // TODO
    void clean_wakeup();

    // 若当前在 loop(I/O) 线程中，则立即执行，反之加入待执行队列，并唤醒 loop 线程
    void runInLoop(Functor cb);
    // 加入待执行队列，并唤醒 loop 线程
    void queueInLoop(Functor cb);
    inline std::size_t queueSize() const { return pendingFunctors_.size(); };
    void doPendingFunctors();

private:
    std::atomic<bool> looping_; // atomic, 由于构造函数保证了一个线程只有一个 EventLoop 对象能够被实例化, 因此不需要用 atomic<bool>?
    std::atomic<bool> quit_; // 是否可以退出循环
    const pid_t threadId_; // 创建时的线程的 pid

    int wakeupFd_; // 用于唤醒的 fd
    std::unique_ptr<Channel> wakeupChannel_; // 用于唤醒的 channel

    using ChannelList = std::vector<Channel*>;
    ChannelList activeChannels_; // 有事件发生的 I/O 队列
    Channel* currentActiveChannel_; // 当前关注的 I/O 事件

    std::unique_ptr<Poller> poller_; // 负责 I/O 复用
    
    std::mutex mutex_; // for pendingFunctors_
    std::vector<Functor> pendingFunctors_; // 额外的待调用函数队列
    std::atomic<bool> callingPendingFunctors_;
};

} // namespace net
} // namespace TCB
#endif // TCB_NET_EVENTLOOP_H