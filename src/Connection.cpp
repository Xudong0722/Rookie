#include "Connection.h"
/*
 * @Author: Xudong0722
 * @Date: 2024-09-18 19:44:49
 * @Last Modified by: Xudong0722
 * @Last Modified time: 2024-11-12 00:34:55
 */
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cassert>
#include <iostream>
#include "Buffer.h"
#include "Channel.h"
#include "Connection.h"
#include "Socket.h"
#include "util.h"

#define READ_BUFFER 1024

Connection::Connection(EventLoop *loop, Socket *sock)
    : event_loop_(loop), sock_(sock), channel_(nullptr), read_buffer_(nullptr) {
  if (nullptr != loop) {
    channel_ = new Channel(loop, sock_->get_fd());
    channel_->enable_reading();
    channel_->use_ET();
  }

  read_buffer_ = new Buffer();
  send_buffer_ = new Buffer();
  state_ = STATE::CONNECTED;
}

Connection::~Connection() {
  if (nullptr != event_loop_) {
    delete channel_;
  }
  delete sock_;
  delete read_buffer_;
  delete send_buffer_;
}

void Connection::read() {
  assert(state_ == STATE::CONNECTED);
  read_buffer_->clear();
  if (sock_->is_non_blocking()) {
    read_non_blocking();
  } else {
    read_blocking();
  }
}

void Connection::write() {
  assert(state_ == STATE::CONNECTED);
  if (sock_->is_non_blocking()) {
    write_non_blocking();
  } else {
    write_blocking();
  }
  send_buffer_->clear();
}

void Connection::read_non_blocking() {
  int sockfd = sock_->get_fd();
  char buf[READ_BUFFER];
  while (true) {
    bzero(&buf, sizeof(buf));
    ssize_t bytes_read = ::read(sockfd, buf, sizeof(buf));
    // std::cout << __PRETTY_FUNCTION__ << " " << bytes_read << std::endl;
    if (bytes_read > 0) {
      // printf("message from client fd: %d, message: %s\n", sockfd, buf);
      //  write(sockfd, buf, sizeof(buf));
      read_buffer_->append(buf, bytes_read);
    } else if (bytes_read == -1 && errno == EINTR) {
      printf("continue reading.");
      continue;
    } else if (bytes_read == -1 && ((errno == EAGAIN) || (errno == EWOULDBLOCK))) {
      // printf("finish reading once, errno: %d\n", errno);
      // printf("message from client fd %d: %s\n", sockfd, read_buffer_->c_str());
      // errif(write(sockfd, read_buffer_->c_str(), read_buffer_->size()) == -1, "socket write error");
      // read_buffer_->clear();
      break;
    } else if (bytes_read == 0) {
      printf("EOF, client fd %d disconnected\n", sockfd);
      // close(sockfd);
      state_ = STATE::CLOSED;
      // delete_connection_callback_(sock_);
      break;
    } else {
      state_ = STATE::CLOSED;
      break;
    }
  }
}

void Connection::write_non_blocking() {
  int sockfd = sock_->get_fd();
  char buf[send_buffer_->size()];
  memcpy(buf, send_buffer_->c_str(), send_buffer_->size());
  int data_size = send_buffer_->size();
  int data_left = data_size;
  while (data_left > 0) {
    ssize_t bytes_write = ::write(sockfd, buf + data_size - data_left, data_left);
    if (bytes_write == -1 && errno == EINTR) {
      printf("continue writing\n");
      continue;
    }
    if (bytes_write == -1 && errno == EAGAIN) {
      break;
    }
    if (bytes_write == -1) {
      printf("Other error on client fd %d\n", sockfd);
      state_ = STATE::CLOSED;
      break;
    }
    data_left -= bytes_write;
  }
}

/*
 * Never used by server, only for client
 */
void Connection::read_blocking() {
  int sockfd = sock_->get_fd();
  unsigned int rcv_size{0};
  socklen_t len = sizeof(rcv_size);
  getsockopt(sockfd, SOL_SOCKET, SO_RCVBUF, &rcv_size, &len);
  char buf[rcv_size];
  ssize_t bytes_read = ::read(sockfd, buf, sizeof(buf));
  if (bytes_read > 0) {
    read_buffer_->append(buf, bytes_read);
  } else if (bytes_read == 0) {
    printf("read EOF, blocking client fd %d disconnected\n", sockfd);
    state_ = STATE::CLOSED;
  } else if (bytes_read == -1) {
    printf("Other error on blocking client fd %d\n", sockfd);
    state_ = STATE::CLOSED;
  }
}

/*
 * Never used by server, only for client
 */
void Connection::write_blocking() {
  int sockfd = sock_->get_fd();
  ssize_t bytes_write = ::write(sockfd, send_buffer_->c_str(), send_buffer_->size());
  if (bytes_write == -1) {
    printf("Other error on blocking client fd:%d\n", sockfd);
    state_ = STATE::CLOSED;
  }
}

void Connection::close() { delete_connection_callback_(sock_); }

Connection::STATE Connection::get_state() { return state_; }

void Connection::set_send_buf(const char *str) { send_buffer_->set_buf(str); }

Buffer *Connection::get_send_buf() { return send_buffer_; }

const char *Connection::send_buf() { return send_buffer_->c_str(); }

Buffer *Connection::get_read_buf() { return read_buffer_; }

const char *Connection::read_buf() { return read_buffer_->c_str(); }

void Connection::set_delete_connection_callback(const std::function<void(Socket *)> &cb) {
  delete_connection_callback_ = cb;
}

void Connection::set_on_connect_callback(const std::function<void(Connection *)> &cb) {
  on_connect_callback_ = cb;
  channel_->set_read_callback([this]() { send_buffer_->getline(); });
}

void Connection::get_line_send_buf() { send_buffer_->getline(); }

Socket *Connection::get_socket() { return sock_; }
