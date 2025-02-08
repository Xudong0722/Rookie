/*
 * @Author: Xudong0722
 * @Date: 2024-09-18 19:44:52
 * @Last Modified by: Xudong0722
 * @Last Modified time: 2024-11-12 00:30:45
 */

#pragma once
#include <functional>
#include <string>
#include "util.h"

class EventLoop;
class Channel;
class Socket;
class Buffer;

class Connection
  : protected noncopymoveable {
 public:
  enum class STATE { INVALID = 0, HANDSHAKING = 1, CONNECTED = 2, CLOSED = 3, FAILED = 4 };

 public:
  Connection(EventLoop *loop, Socket *sock);
  ~Connection();

  void read();
  void write();

  void set_on_connect_callback(const std::function<void(Connection *)> &cb);
  void set_delete_connection_callback(const std::function<void(Socket *)> &cb);

  STATE get_state();
  void close();

  void set_send_buf(const char *str);
  Buffer *get_read_buf();
  const char *read_buf();
  ssize_t get_read_buf_size();
  Buffer *get_send_buf();
  const char *send_buf();
  ssize_t get_send_buf_size();
  void get_line_send_buf();
  Socket *get_socket();

  void on_connect(std::function<void()> fn);

 private:
  void read_non_blocking();
  void write_non_blocking();
  void read_blocking();
  void write_blocking();

 private:
  EventLoop *event_loop_;
  Socket *sock_;
  Channel *channel_;
  Buffer *read_buffer_;
  Buffer *send_buffer_;
  STATE state_;
  std::function<void(Socket *)> delete_connection_callback_;
  std::function<void(Connection *)> on_connect_callback_;
};
