/*
 * @Author: Xudong0722
 * @Date: 2024-09-09 22:31:18
 * @Last Modified by: Xudong0722
 * @Last Modified time: 2024-11-12 01:22:28
 */
#pragma once

#include <map>
#include <vector>

#define READ_BUFFER 1024

class EventLoop;
class Socket;
class Acceptor;
class Connection;
class ThreadPool;

class Server {
 public:
  Server(EventLoop *event_loop);
  ~Server();

  void handle_new_connect_event(Socket *sock);
  void delete_connection(Socket *sock);

 private:
  EventLoop *main_reactor_{nullptr};
  Acceptor *acceptor_{nullptr};
  std::map<int, Connection *> connections_;
  std::vector<EventLoop *> sub_reactors_;
  ThreadPool *thread_pool_{nullptr};
};