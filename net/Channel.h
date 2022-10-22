#ifndef TCB_NET_CHANNEL_H
#define TCB_NET_CHANNEL_H

#include <functional>
#include <memory>
#include "../base/CurrentThread.h"
#include "../base/noncopyable.h"
#include "Poller.h"

namespace TCB
{
namespace net
{

class EventLoop;

// 负责注册和响应 IO 事件
// 结构类似 pollfd，包含 fd, events, and revents
// fd 与唯一一个 EventLoop 绑定，即只会有一个线程负责该 fd 的 I/O 事件
// 但是该类不拥有文件描述符, 不负责关闭文件描述符
class Channel
{
public:
    using EventCallBack = std::function<void ()>;

    Channel(EventLoop *loop, int fd);
    ~Channel(); // TODO

    // 返回 struct pollfd 的成员

    inline int fd() const { return fd_; }
    inline int events() const { return events_; }
    // inline int revents() const { return revents_; }
    inline void set_revents(int revents) { revents_ = revents; }
    inline int index() const { return index_; }
    inline void set_index(int index) { index_ = index; }

    // 设置监听的事件 (可读 or 可写)

    inline void enableReading() { events_ |= kReadEvent; update(); }
    inline void disableReading() { events_ &= ~kReadEvent; update(); }
    inline void enableWriting() { events_ |= kWriteEvent; update(); }
    inline void disableWriting() { events_ &= ~kWriteEvent; update(); }
    inline void disableAll() { events_ = kNoneEvent; update(); }

    // 检查感兴趣的 I/O 事件

    inline bool isNoneEvent() const { return events_ == kNoneEvent; }
    inline bool isWriting() const { return events_ & kWriteEvent; }
    inline bool isReading() const { return events_ & kReadEvent; }

    // 设置回调函数

    inline void setReadCallBack(const EventCallBack &cb) noexcept { readCallBack = cb; }
    inline void setWriteCallBack(const EventCallBack &cb) noexcept { writeCallBack = cb; }
    inline void setErrorCallBack(const EventCallBack &cb) noexcept { errorCallBack = cb; }
    

private:
    inline void update();

    EventLoop *loop_; // 绑定的 EventLoop
    const int fd_;  // 文件描述符
    int events_;    // 注册的事件
    int revents_;   // 实际发生的事件
    int index_;     // 记录自己在 pollfds_ 中的索引, -1 表示未记录，方便定位和删除

    EventCallBack readCallBack;
    EventCallBack writeCallBack;
    EventCallBack errorCallBack;

    static const int kNoneEvent;
    static const int kReadEvent;
    static const int kWriteEvent;
};


void Channel::update() {
    loop_->updateChannel(this);
}

} // namespace net
} // namespace TCB


#endif // TCB_NET_CHANNEL_H