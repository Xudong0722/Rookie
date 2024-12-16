/*
 * @Author: Xudong0722
 * @Date: 2024-09-09 15:20:27
 * @Last Modified by: Xudong0722
 * @Last Modified time: 2024-09-09 23:09:00
 */

#pragma once
#include <stdint.h>
#include <functional>

class Epoll;
class EventLoop;

class Channel {
 public:
  Channel(EventLoop *event_loop, int fd);
  ~Channel();

  void enable_reading();
  void handle_event();

  int get_fd();
  uint32_t get_events() const;
  void set_events(uint32_t events);
  uint32_t get_ready() const;
  void set_ready(uint32_t ready);

  bool get_in_epoll() const;
  void set_in_epoll(bool is_in_epoll);

  void use_ET();
  void set_use_threadpool(bool use = true);
  void set_read_callback(std::function<void()> cb);

 private:
  EventLoop *event_loop_;
  int fd_{-1};

  uint32_t events_{0};
  uint32_t ready_{0};
  bool is_in_epoll_{false};
  bool is_use_threadpool_{false};
  std::function<void()> read_cb_;
  std::function<void()> write_cb_;
};