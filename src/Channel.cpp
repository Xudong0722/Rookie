/*
 * @Author: Xudong0722
 * @Date: 2024-09-09 15:25:45
 * @Last Modified by: Xudong0722
 * @Last Modified time: 2024-11-12 21:09:24
 */
#include "Channel.h"
#include <sys/epoll.h>
#include <unistd.h>
#include "EventLoop.h"
#include "Socket.h"

Channel::Channel(EventLoop *event_loop, int fd) : event_loop_(event_loop), fd_(fd) {}

Channel::~Channel() {
  if (fd_ != -1) {
    close(fd_);
    fd_ = -1;
  }
}

void Channel::enable_reading() {
  listen_events_ |= EPOLLIN | EPOLLPRI;
  event_loop_->update_channel(this);
}

void Channel::use_ET() {
  listen_events_ |= EPOLLET;
  event_loop_->update_channel(this);
}

void Channel::handle_event() {
  if (ready_events_ & (EPOLLIN | EPOLLPRI)) {
    read_cb_();
  } else if (ready_events_ & EPOLLOUT) {
    write_cb_();
  }
}

int Channel::get_fd() { return fd_; }

uint32_t Channel::get_listen_events() const { return listen_events_; }

uint32_t Channel::get_ready_events() const { return ready_events_; }

void Channel::set_ready_events(uint32_t ready) { ready_events_ = ready; }

bool Channel::get_in_epoll() const { return is_in_epoll_; }

void Channel::set_in_epoll(bool is_in_epoll) { is_in_epoll_ = is_in_epoll; }

void Channel::set_read_callback(const std::function<void()> &cb) {
  if (nullptr == cb) return;
  read_cb_ = cb;
}
