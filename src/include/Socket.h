/*
 * @Author: Xudong0722
 * @Date: 2024-09-08 12:45:07
 * @Last Modified by: Xudong0722
 * @Last Modified time: 2024-11-12 01:22:31
 */

#pragma once
#include <stdint.h>
#include "util.h"

class InetAddr;

class Socket
  : protected noncopymoveable {
 public:
  Socket();
  explicit Socket(int _fd);
  ~Socket();

 public:
  void bind(InetAddr *);
  void listen();
  void set_non_blocking();
  bool is_non_blocking();
  int accept(InetAddr *);
  void connect(InetAddr *);
  void connect(const char *ip, uint16_t port);
  int get_fd();

 private:
  int fd_;
};
