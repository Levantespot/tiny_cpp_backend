#include "Channel.h"
#include "poll.h"

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
    readCallBack(),
    writeCallBack(),
    errorCallBack()
{
    // empty
}

Channel::~Channel()
{
}    

} // namespace net
} // namespace TCB