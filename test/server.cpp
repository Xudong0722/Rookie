/*
 * @Author: Xudong0722
 * @Date: 2024-09-01 12:57:41
 * @Last Modified by: Xudong0722
 * @Last Modified time: 2025-02-22 16:53:58
 */

#include <iostream>
#include <string>
#include "TcpServer.h"
#include "Buffer.h"
#include "Connection.h"
#include "EventLoop.h"
#include "Socket.h"

int main() {
  EventLoop *loop = new EventLoop();
  TcpServer *server = new TcpServer(loop);

  server->on_connect([](Connection *conn) {
    conn->read();
    if (conn->get_state() == Connection::STATE::CLOSED) {
      conn->close();
      return;
    }
    std::cout << "Message from client " << conn->get_socket()->get_fd() << ": " << conn->read_buf() << std::endl;
    std::string client_data = conn->read_buf();
    std::cout << client_data.size() <<" " << client_data.length() << std::endl;
    for(int i = 0; i<client_data.length(); ++i){
      if(client_data[i] == '.') client_data[i] = '*';
    }
    conn->set_send_buf(client_data.c_str());
    conn->write();
  });
  loop->loop();

  delete loop;
  delete server;
  return 0;
}