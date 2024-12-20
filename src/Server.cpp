/*
 * @Author: Xudong0722
 * @Date: 2024-09-09 23:00:21
 * @Last Modified by: Xudong0722
 * @Last Modified time: 2024-09-18 22:58:13
 */

#include "Server.h"
#include <unistd.h>
#include <functional>
#include "Acceptor.h"
#include "Channel.h"
#include "Connection.h"
#include "EventLoop.h"
#include "InetAddr.h"
#include "Socket.h"
#include "ThreadPool.h"

Server::Server(EventLoop *event_loop) : main_reactor_(event_loop) {
  acceptor_ = new Acceptor(main_reactor_);
  std::function<void(Socket *)> cb = std::bind(&Server::handle_new_connect_event, this, std::placeholders::_1);
  acceptor_->set_new_connection_callback(cb);

  int threads_limit = std::thread::hardware_concurrency();
  thread_pool_ = new ThreadPool(threads_limit);

  for (int i = 0; i < threads_limit; ++i) {
    sub_reactors_.emplace_back(new EventLoop());
  }

  for (int i = 0; i < threads_limit; ++i) {
    // One loop per thread
    std::function<void()> sub_loop = std::bind(&EventLoop::loop, sub_reactors_[i]);
    thread_pool_->add(sub_loop);
  }
}

Server::~Server() {
  delete acceptor_;
  delete thread_pool_;
}

void Server::handle_new_connect_event(Socket *sock) {
  if (nullptr == sock || sock->get_fd() == -1) return;

  int reactor_index = sock->get_fd() % sub_reactors_.size();
  Connection *conn = new Connection(sub_reactors_[reactor_index], sock);
  std::function<void(Socket *)> cb = std::bind(&Server::delete_connection, this, std::placeholders::_1);
  conn->set_delete_connection_callback(cb);
  conn->set_on_connect_callback(on_connect_callback_);
  connections_[sock->get_fd()] = conn;
}

void Server::delete_connection(Socket *sock) {
  if (nullptr == sock || sock->get_fd() == -1) return;
  auto it = connections_.find(sock->get_fd());
  if (it == connections_.end()) return;

  close(sock->get_fd());
  auto tmp = connections_[sock->get_fd()];
  connections_.erase(sock->get_fd());
  delete tmp;
}

void Server::on_connect(std::function<void(Connection *)> cb) { on_connect_callback_ = std::move(cb); }