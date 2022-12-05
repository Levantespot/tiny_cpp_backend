#include "SocketOps.h"

namespace TCB
{
namespace net
{
namespace socket
{
  
int  connect(int sockfd, const struct sockaddr* addr) {}
void bindOrDie(int sockfd, const struct sockaddr* addr) {}
void listenOrDie(int sockfd) {}
int  accept(int sockfd, struct sockaddr_in6* addr) {}
ssize_t read(int sockfd, void *buf, size_t count) {}
ssize_t readv(int sockfd, const struct iovec *iov, int iovcnt) {}
ssize_t write(int sockfd, const void *buf, size_t count) {}
void close(int sockfd) {}
void shutdownWrite(int sockfd) {}

void toIpPort(char* buf, size_t size, const struct sockaddr* addr) {}
void toIp(char* buf, size_t size, const struct sockaddr* addr) {}

const struct sockaddr* sockaddr_cast(const struct sockaddr_in* addr) {
  return reinterpret_cast<const struct sockaddr*>(addr);
}
const struct sockaddr* sockaddr_cast(const struct sockaddr_in6* addr) {
  return reinterpret_cast<const struct sockaddr*>(addr);
}

const struct sockaddr_in* sockaddr_in_cast(const struct sockaddr* addr) {
  return reinterpret_cast<const struct sockaddr_in*>(addr);
}
const struct sockaddr_in6* sockaddr_in6_cast(const struct sockaddr* addr) {
  return reinterpret_cast<const struct sockaddr_in6*>(addr);
}

struct sockaddr_in6 getLocalAddr(int sockfd) {

}
struct sockaddr_in6 getPeerAddr(int sockfd) {

}

} // namespace socket
} // namespace net
} // namespace TCB
