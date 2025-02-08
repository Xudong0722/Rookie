/*
 * @Author: Xudong0722
 * @Date: 2024-09-01 12:57:41
 * @Last Modified by: Xudong0722
 * @Last Modified time: 2024-12-27 00:23:46
 */
#include "TcpServer.h"
#include <iostream>
#include "Buffer.h"
#include "Connection.h"
#include "EventLoop.h"
#include "Socket.h"
#include "Elog.h"

int main() {
  Elog::GetInst().set_log_file("server.txt");  
  EventLoop *loop = new EventLoop();
  TcpServer *server = new TcpServer(loop);

  server->on_connect([](Connection *conn) {
    conn->read();
    if (conn->get_state() == Connection::STATE::CLOSED) {
      conn->close();
      return;
    }
    std::cout << "Message from client " << conn->get_socket()->get_fd() << ": " << conn->read_buf() << std::endl;
    conn->set_send_buf(conn->read_buf());
    conn->write();
  });
  loop->loop();

  delete loop;
  delete server;
  return 0;
}