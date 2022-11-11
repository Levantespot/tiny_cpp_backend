#include "net/poller/PollPoller.h"

#include <assert.h>
#include <poll.h>

#include "base/Logger.h"
#include "net/Channel.h"

namespace TCB {
namespace net {

PollPoller::PollPoller(EventLoop* loop) : Poller(loop) {
  // empty
}

Timestamp PollPoller::poll(int timeoutMs, ChannelList& activeChannels) {
  int numEvents = ::poll(pollfds_.data(), pollfds_.size(), timeoutMs);
  int savedErrno = errno;
  Timestamp now;
  if (numEvents > 0) {
    LOG_TRACE << numEvents << " events happened.";
    fillActiveChannels(numEvents, activeChannels);
  } else if (numEvents == 0) {
    LOG_TRACE << "Nothing happened.";
  } else {
    if (savedErrno != EINTR) {
      errno = savedErrno;
      LOG_SYSERR << "PollPoller::poll errored, errno = " << savedErrno;
    }
  }
  return now;
}

void PollPoller::fillActiveChannels(int numEvents, ChannelList& activeChannels) const {
  for (const auto& pfd : pollfds_) {
    if (pfd.revents > 0) {
      --numEvents;
      auto iter = channelMaps_.find(pfd.fd);
      assert(iter != channelMaps_.end());
      Channel* channel = iter->second;
      assert(channel->fd() == pfd.fd);
      channel->set_revents(pfd.revents);
      activeChannels.push_back(channel);
    }
  }
}

void PollPoller::updateChannel(Channel* channel) {
  Poller::assertInLoopThread();
  LOG_TRACE << "fd = " << channel->fd() << " events = " << channel->events();
  if (channel->index() < 0) {
    assert(channelMaps_.find(channel->fd()) == channelMaps_.end());
    // create & insert `struct pollfd`
    struct pollfd pfd;
    pfd.fd = channel->fd();
    pfd.events = static_cast<short>(channel->events());
    pfd.revents = 0;
    pollfds_.push_back(pfd);
    // 记录 channel 在 pollfds_ 中的索引
    int idx = static_cast<int>(pollfds_.size()) - 1;
    channel->set_index(idx);
    // insert it to maps
    channelMaps_[pfd.fd] = channel;
  } else {
    // update existing one
    assert(channelMaps_[channel->fd()] == channel);
    struct pollfd& pfd = pollfds_[channel->index()];
    assert(pfd.fd == channel->fd() || pfd.fd == -channel->fd() - 1);
    pfd.fd = channel->fd();
    pfd.events = channel->events();
    pfd.revents = 0;

    if (channel->isNoneEvent()) {
      // ignore this pollfd
      pfd.fd = -channel->fd() - 1;
    }
  }
}

void PollPoller::removeChannel(Channel* channel) {
  Poller::assertInLoopThread();
  LOG_TRACE << "fd = " << channel->fd();
  auto iter = channelMaps_.find(channel->fd());
  assert(iter != channelMaps_.end());
  assert(iter->second == channel);
  assert(channel->isNoneEvent());

  // 从 channelMaps_ 中删除
  channelMaps_.erase(iter);

  // 从 pollfds_ 中删除
  int idx = channel->index();
  if (static_cast<size_t>(idx) == pollfds_.size() - 1) {
    // 最后一个位置，直接删除
    pollfds_.pop_back();
  } else {
    // 用最后一个元素替代
    int channelAtEnd = pollfds_.back().fd;
    std::iter_swap(pollfds_.begin() + idx, pollfds_.end() - 1);
    pollfds_.pop_back();
    // 更新 channelMaps_
    if (channelAtEnd < 0) {
      channelAtEnd = -channelAtEnd - 1;  // 还原
    }
    channelMaps_[channelAtEnd]->set_index(idx);
  }
}

}  // namespace net
}  // namespace TCB
