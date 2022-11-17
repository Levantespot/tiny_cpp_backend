#include "InetAddress.h"

#include <arpa/inet.h>  // inet_pton
#include <assert.h>
#include <string.h>

// #include "base/Logger.h"

namespace TCB {
namespace net {

InetAddress::InetAddress(uint16_t port, bool loopbackOnly, bool ipv6) {
  if (ipv6) {
    memset(&addr6_, 0, sizeof addr6_);
    addr6_.sin6_family = AF_INET6;
    addr6_.sin6_port = htons(port);
    addr6_.sin6_addr = loopbackOnly ? in6addr_loopback : in6addr_any;
  } else {
    memset(&addr_, 0, sizeof addr_);
    addr_.sin_family = AF_INET;
    addr_.sin_port = htons(port);
    addr_.sin_addr.s_addr = loopbackOnly ? htonl(INADDR_LOOPBACK) : INADDR_ANY;
  }
}

InetAddress::InetAddress(const std::string ip, uint16_t port, bool ipv6) {
  if (ipv6) {
    memset(&addr6_, 0, sizeof addr6_);
    addr6_.sin6_family = AF_INET6;
    addr6_.sin6_port = htons(port);
    inet_pton(AF_INET6, ip.c_str(), &addr6_.sin6_addr);
  } else {
    memset(&addr_, 0, sizeof addr_);
    addr_.sin_family = AF_INET;
    addr_.sin_port = htons(port);
    inet_pton(AF_INET, ip.c_str(), &addr_.sin_addr);
  }
}

std::string InetAddress::toIp() const {
  char buf[64] = "";
  if (addr_.sin_family == AF_INET) {
    inet_ntop(AF_INET, &addr_.sin_addr, buf, static_cast<socklen_t>(sizeof buf));
  } else {
    inet_ntop(AF_INET6, &addr6_.sin6_addr, buf, static_cast<socklen_t>(sizeof buf));
  }
  return buf;
}

std::string InetAddress::toIpPort() const {
  char buf[64] = "";
  if (addr_.sin_family == AF_INET) {
    inet_ntop(AF_INET, &addr_.sin_addr, buf, static_cast<socklen_t>(sizeof buf));
  } else {
    inet_ntop(AF_INET6, &addr6_.sin6_addr, buf, static_cast<socklen_t>(sizeof buf));
  }
  std::size_t len = strlen(buf);
  snprintf(buf+len, sizeof buf - len, ":%d", addr_.sin_port);
  return buf;
}

}  // namespace net
}  // namespace TCB
