#include "Socket.h"

#include <sys/socket.h>
#include <unistd.h>

#include "InetAddress.h"


namespace TCB
{
namespace net
{

Socket::~Socket() {
  ::close(sockfd_);
}

void Socket::bindAddress(const InetAddress& localAddr) {
  // ::bind(sockfd_, )
}

void Socket::listen() {

}

int Socket::accept(InetAddress* peerAddr) {

}

void Socket::shutdownWrite() {

}

} // namespace net
} // namespace TCB
