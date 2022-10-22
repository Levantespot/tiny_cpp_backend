#ifndef TCB_NET_POLLPOLLER_H
#define TCB_NET_POLLPOLLER_H

#include "../Poller.h"
#include <vector>

struct pollfd;

namespace TCB
{
namespace net
{
    
class PollPoller : public Poller {

public:

    PollPoller(EventLoop* loop);
    ~PollPoller() override = default;

    // 调用 <poll.h> 中的 poll，并调用 fillActiveChannels 更新 activeChannels
    Timestamp poll(int timeoutMs, ChannelList& activeChannels) override;

    // 监听 channel 中的 fd & 更新 pollfds_
    void updateChannel(Channel* channel) override;
    // 移除 channel 中的 fd & 更新 pollfds_
    void removeChannel(Channel* channel) override; 

private:
    // 遍历 pollfds_，找到 numEvents 个有活动的事件，
    // 将其对应的 channel* 写入 activeChannels
    void fillActiveChannels(int numEvents, ChannelList& activeChannels) const;

    using PollFdList = std::vector<struct pollfd>;
    PollFdList pollfds_;
};


} // namespace net
} // namespace TCB

#endif // TCB_NET_POLLPOLLER_H