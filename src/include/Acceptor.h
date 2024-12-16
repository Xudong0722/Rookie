/*
 * @Author: Xudong0722
 * @Date: 2024-09-18 17:25:25
 * @Last Modified by: Xudong0722
 * @Last Modified time: 2024-09-18 17:29:22
 */

#pragma once
#include <functional>

class EventLoop;
class Socket;
class InetAddr;
class Channel;

class Acceptor {
 public:
  Acceptor(EventLoop *loop);
  ~Acceptor();

  void accept_connection();

  void set_new_connection_callback(std::function<void(Socket *)>);

 private:
  EventLoop *loop_{nullptr};
  Socket *sock_{nullptr};
  Channel *accept_channel_{nullptr};
  std::function<void(Socket *)> new_connection_callback_;
};