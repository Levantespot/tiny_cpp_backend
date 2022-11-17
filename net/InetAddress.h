#ifndef TCB_NET_INETADDRESS_H
#define TCB_NET_INETADDRESS_H

#include <netinet/in.h>

#include <string>

#include "base/copyable.h"

namespace TCB {
namespace net {

class InetAddress : copyable {
 public:
  // Constructs an endpoint with given port number.
  // Mostly used in TcpServer listening.
  explicit InetAddress(uint16_t port = 0, bool loopbackOnly = false, bool ipv6 = false);

  // Constructs an endpoint with given ip and port.
  // @c ip should be "1.2.3.4"
  InetAddress(const std::string ip, uint16_t port, bool ipv6 = false);

  // Constructs an endpoint with given struct @c sockaddr_in
  // Mostly used when accepting new connections
  explicit InetAddress(const struct sockaddr_in& addr) : addr_(addr) {}
  explicit InetAddress(const struct sockaddr_in6& addr) : addr6_(addr) {}

  inline sa_family_t family() const { return addr_.sin_family; }
  inline uint16_t port() const { return addr_.sin_port; }
  std::string toIp() const;
  std::string toIpPort() const;

 private:
  union {
    struct sockaddr_in addr_;
    struct sockaddr_in6 addr6_;
  };
};

}  // namespace net
}  // namespace TCB

#endif  // TCB_NET_INETADDRESS_H