#ifndef TCB_NET_EVENTLOOP_H
#define TCB_NET_EVENTLOOP_H

#include <vector>
#include <functional>
#include <atomic>
#include <memory>

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
    EventLoop();
    ~EventLoop();

    void loop(); // 在循环中不断处理业务 TODO
    inline void quit() { quit_ = true; } // 通知停止循环
    static EventLoop* getEventLoopOfCurrentThread();

    inline bool isInLoopThread () const { return threadId_ == CurrentThread::tid(); }
    void assertInLoopThread() const; // 确保当前线程是 EventLoop 的线程

    /* 更新、移除、判断 poller 中的 map<int, channel> */

    void updateChannel(Channel* channel);
    void removeChannel(Channel* channel);
    bool hasChannel(Channel* channel);

private:
    std::atomic<bool> looping_; // atomic, 由于构造函数保证了一个线程只有一个 EventLoop 对象能够被实例化, 因此不需要用 atomic<bool>?
    std::atomic<bool> quit_; // 是否可以退出循环
    const pid_t threadId_; // 创建时的线程的 pid

    using ChannelList = std::vector<Channel*>;
    ChannelList activeChannels_; // 有事件发生的 I/O 队列
    Channel* currentActiveChannel_; // 当前关注的 I/O 事件

    std::unique_ptr<Poller> poller_; // 负责 I/O 复用

};

} // namespace net
} // namespace TCB
#endif // TCB_NET_EVENTLOOP_H