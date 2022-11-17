#include <functional>
#include <iostream>
#include <string>
#include <vector>

#include "net/InetAddress.h"

int main() {
  const char* ipv4 = "112.113.114.115";
  const char* ipv6 = "240c:c983:5:16df:ca73:f484:76aa:634f";
  in_port_t port = 6789;

  TCB::net::InetAddress ipv4_addr(ipv4, port, false);
  TCB::net::InetAddress ipv4_addr_without_ip(port, false, false);
  TCB::net::InetAddress ipv4_addr_loopback(port, true, false);

  std::cout << ipv4_addr.toIp() << '\n'
            << ipv4_addr_without_ip.toIp() << '\n'
            << ipv4_addr_loopback.toIp() << '\n';

  TCB::net::InetAddress ipv6_addr(ipv6, port, true);
  TCB::net::InetAddress ipv6_addr_without_ip(port, false, true);
  TCB::net::InetAddress ipv6_addr_loopback(port, true, true);

  std::cout << ipv6_addr.toIp() << '\n'
            << ipv6_addr_without_ip.toIp() << '\n'
            << ipv6_addr_loopback.toIp() << '\n';

  return 0;
}