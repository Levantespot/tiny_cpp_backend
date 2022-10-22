#ifndef TCB_NET_POLLER_H
#define TCB_NET_POLLER_H

#include "EventLoop.h"
#include "../base/Timestamp.h"
#include <vector>
#include <unordered_map>

namespace TCB
{
namespace net
{

class Channel;

// 负责 I/O 复用的抽象基类.
// 可以有 poll 或 epoll 的具体实现.
class Poller : noncopyable
{
public:
    using ChannelList = std::vector<Channel*>;

    Poller(EventLoop* loop);
    virtual ~Poller() = default;

    virtual Timestamp poll(int timeoutMs, ChannelList& activeChannels) = 0; // 发起 poll

     // 负责修改感兴趣的 I/O 事件 & channel
    virtual void updateChannel(Channel* channel) = 0;
    // 负责移除感兴趣的 I/O 事件 & channel
    virtual void removeChannel(Channel* channel) = 0;
    // 判断该 poller 中是否负责参数的 channel
    virtual bool hasChannel(Channel* channel) const;

    static Poller* newDefaultPoller(EventLoop *loop); // TODO

    inline void assertInLoopThread() const { ownerLoop_->assertInLoopThread(); }

protected:
    using ChannelMap = std::unordered_map<int, Channel*>;
    ChannelMap channelMaps_; // 缓存每个 fd 和 Channel* 的对应关系
private:
    EventLoop* ownerLoop_;
};

} // namespace net
} // namespace TCB


#endif