/*
 * @Author: Xudong0722
 * @Date: 2024-09-18 19:44:52
 * @Last Modified by: Xudong0722
 * @Last Modified time: 2024-11-12 00:30:45
 */

#pragma once
#include <functional>
#include <string>

class EventLoop;
class Channel;
class Socket;
class Buffer;

class Connection {
 public:
  Connection(EventLoop *loop, Socket *sock);
  ~Connection();

  void echo(int sockfd);
  void set_delete_connection_callback(std::function<void(Socket *)>);
  void send(int sockfd);

 private:
  EventLoop *event_loop_;
  Socket *sock_;
  Channel *channel_;
  Buffer *read_buffer_;
  std::function<void(Socket *)> delete_connection_callback_;
};