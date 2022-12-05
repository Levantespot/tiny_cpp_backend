#ifndef TCB_NET_SOCKET_H
#define TCB_NET_SOCKET_H

#include "base/noncopyable.h"

namespace TCB {
namespace net {

class InetAddress;

class Socket : noncopyable {
 public:
  explicit Socket(int sockfd) : sockfd_(sockfd) {}

  ~Socket();

  int fd() const { return sockfd_; }

  void bindAddress(const InetAddress& localAddr);
  void listen();
  int accept(InetAddress* peerAddr);
  
  void shutdownWrite();

 private:
  int sockfd_;
};

}  // namespace net
}  // namespace TCB

#endif  // TCB_NET_SOCKET_H