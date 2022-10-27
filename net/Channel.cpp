#include "Channel.h"
#include "poll.h"
#include "../base/Logger.h"

namespace TCB
{
namespace net
{

const int Channel::kNoneEvent = 0;
const int Channel::kReadEvent = POLLIN | POLLPRI; // 普通/优先数据 + 高优先数据可读
const int Channel::kWriteEvent = POLLOUT; // 普通/优先数据可写

Channel::Channel(EventLoop *loop, int fd)
  : loop_(loop),
    fd_(fd),
    events_(0),
    revents_(0),
    index_(-1),
    readCallback_(),
    writeCallback_(),
    closeCallback_(),
    errorCallback_()
{
    // empty
}

Channel::~Channel()
{
}    

void Channel::handleEvent() {
    LOG_TRACE << reventsToString();
    // 若无数据要读，且对方端关闭
    if((revents_ & POLLHUP) && !(revents_ & POLLIN)) {
        if (closeCallback_) {
            closeCallback_();
        }
    }
    // 若 fd 未打开
    if (revents_ & POLLNVAL) {
        LOG_WARN << "fd = " << fd_ << " Channel::handle_event() POLLNVAL";
    }
    // 出错处理
    if (revents_ & (POLLERR | POLLNVAL)) {
        if (errorCallback_) {
            errorCallback_();
        }
    }
    // 若有数据可读
    if (revents_ & (POLLIN | POLLPRI | POLLRDHUP)) {
        if (readCallback_) {
            readCallback_();
        }
    }
    // 若有数据可写
    if (revents_ & POLLOUT) {
        if (writeCallback_) {
            writeCallback_();
        }
    }
}

std::string Channel::reventsToString() const {
    return eventsToString(fd_, revents_);
}
std::string Channel::eventsToString() const {
    return eventsToString(fd_, events_);
}
std::string Channel::eventsToString(int fd, int ev)
{
    std::ostringstream oss;
    oss << "fd " << fd << ": ";
    if (ev & POLLIN)
        oss << "IN ";
    if (ev & POLLPRI)
        oss << "PRI ";
    if (ev & POLLOUT)
        oss << "OUT ";
    if (ev & POLLHUP)
        oss << "HUP ";
    if (ev & POLLRDHUP)
        oss << "RDHUP ";
    if (ev & POLLERR)
        oss << "ERR ";
    if (ev & POLLNVAL)
        oss << "NVAL ";

    return oss.str();
}

} // namespace net
} // namespace TCB