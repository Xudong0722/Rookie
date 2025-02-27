/*
 * @Author: Xudong0722
 * @Date: 2024-09-07 22:33:36
 * @Last Modified by: Xudong0722
 * @Last Modified time: 2024-11-12 01:16:36
 */
#pragma once
#include <arpa/inet.h>
#include "util.h"

class InetAddr
  : protected noncopymoveable {
 public:
  InetAddr();
  InetAddr(const char *ip, uint16_t port);
  ~InetAddr();

 public:
  void set_inet_addr(sockaddr_in addr, socklen_t addr_len);
  sockaddr_in get_addr();
  socklen_t get_addr_len();

 public:
  struct sockaddr_in addr_info_;
  socklen_t addr_len_;
};
