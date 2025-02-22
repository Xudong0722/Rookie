/*
 * @Author: Xudong0722
 * @Date: 2024-09-01 12:57:55
 * @Last Modified by: Xudong0722
 * @Last Modified time: 2024-12-29 13:21:00
 */
#include <iostream>
#include "Connection.h"
#include "Socket.h"

int main() {
  Socket *sock = new Socket();
  sock->connect("127.0.0.1", 8888);

  Connection *conn = new Connection(nullptr, sock);
  while (true) {
    conn->get_line_send_buf();
    
    if(conn->get_send_buf_size() > 0){
      conn->write();
      if (conn->get_state() == Connection::STATE::CLOSED) {
        conn->close();
        break;
      }
      conn->read();
      std::cout << "Message from server: " << conn->read_buf() << std::endl;
    }
  }

  delete sock;
  delete conn;
  return 0;
}