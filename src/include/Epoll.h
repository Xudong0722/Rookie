/*
 * @Author: Xudong0722
 * @Date: 2024-09-08 13:26:07
 * @Last Modified by: Xudong0722
 * @Last Modified time: 2024-11-10 17:06:02
 */
#pragma once
#include <sys/epoll.h>
#include <vector>
#include "util.h"

class Channel;

class Epoll
  : protected noncopymoveable {
 private:
  int epfd_;
  struct epoll_event *events_;

 public:
  Epoll();
  ~Epoll();

  // void add_fd(int fd, uint32_t op, Channel *channel);
  void update_channel(Channel *channel);
  void delete_channel(Channel *channel);
  std::vector<Channel *> wait(int timeout = -1);
};
