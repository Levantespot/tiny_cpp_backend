#include "Poller.h"
#include "Channel.h"

namespace TCB
{
namespace net
{

Poller::Poller(EventLoop* loop)
  : channelMaps_(),
    ownerLoop_(loop)
{
    // empty
}

bool Poller::hasChannel(Channel* channel) const {
    assertInLoopThread();
    const auto iter = channelMaps_.find(channel->fd());
    return iter != channelMaps_.end() && iter->second == channel;
}

} // namespace net
} // namespace TCB
