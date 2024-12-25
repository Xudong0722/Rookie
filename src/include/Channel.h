/*
 * @Author: Xudong0722
 * @Date: 2024-09-09 15:20:27
 * @Last Modified by: Xudong0722
 * @Last Modified time: 2024-09-09 23:09:00
 */

#pragma once
#include <stdint.h>
#include <functional>
#include "util.h"

class Epoll;
class EventLoop;

class Channel
  : protected noncopymoveable {
 public:
  Channel(EventLoop *event_loop, int fd);
  ~Channel();

  void enable_reading();
  void handle_event();

  int get_fd();
  uint32_t get_listen_events() const;

  uint32_t get_ready_events() const;
  void set_ready_events(uint32_t ready);

  bool get_in_epoll() const;
  void set_in_epoll(bool is_in_epoll);

  void use_ET();
  void set_read_callback(const std::function<void()>& cb);

 private:
  EventLoop *event_loop_;
  int fd_{-1};

  uint32_t listen_events_{0};
  uint32_t ready_events_{0};
  bool is_in_epoll_{false};
  std::function<void()> read_cb_;
  std::function<void()> write_cb_;
};
