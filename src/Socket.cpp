/*
 * @Author: Xudong0722
 * @Date: 2024-09-08 12:49:44
 * @Last Modified by: Xudong0722
 * @Last Modified time: 2024-11-12 01:23:01
 */

#include "Socket.h"
#include <fcntl.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cerrno>
#include "InetAddr.h"
#include "util.h"

Socket::Socket() : fd_(-1) {
  fd_ = socket(AF_INET, SOCK_STREAM, 0);
  errif(fd_ == -1, "Create socket failed.");
}

Socket::Socket(int _fd) : fd_(_fd) { errif(fd_ == -1, "Create socket failed."); }

Socket::~Socket() {
  if (fd_ != -1) {
    close(fd_);
    fd_ = -1;
  }
}

void Socket::bind(InetAddr *addr_info) {
  errif(NULL == addr_info, "addr_info is nullptr");
  errif(::bind(fd_, (sockaddr *)addr_info, addr_info->addr_len_) == -1, "bind socket failed.");
}

void Socket::listen() { errif(::listen(fd_, SOMAXCONN) == -1, "Socket listen failed."); }

void Socket::set_non_blocking() { fcntl(fd_, F_SETFL, fcntl(fd_, F_GETFL) | O_NONBLOCK); }

bool Socket::is_non_blocking() { return (fcntl(fd_, F_GETFL) & O_NONBLOCK) != 0; }

int Socket::accept(InetAddr *addr_info) {
  struct sockaddr_in addr;
  socklen_t addr_len = sizeof(addr);
  int client_sockfd{-1};
  if (is_non_blocking()) {
    while (true) {
      client_sockfd = ::accept(fd_, (sockaddr *)&addr, &addr_len);
      if (client_sockfd == -1 && ((errno == EAGAIN) || errno == EWOULDBLOCK)) {
        continue;
      }

      if (client_sockfd == -1) {
        errif(true, "socket accept error");
      } else {
        break;
      }
    }
  } else {
    client_sockfd = ::accept(fd_, (sockaddr *)&addr, &addr_len);
    errif(client_sockfd == -1, "Socket accept error....");
  }
  addr_info->set_inet_addr(addr, addr_len);
  return client_sockfd;
}

void Socket::connect(InetAddr *addr) {
  struct sockaddr_in addr_info = addr->get_addr();
  if (fcntl(fd_, F_GETFL) & O_NONBLOCK) {
    while (true) {
      int ret = ::connect(fd_, (sockaddr *)&addr_info, sizeof(addr_info));
      if (ret == 0) break;
      if (ret == -1 && errno == EINPROGRESS) {
        continue;
      }
      if (ret == -1) {
        errif(true, "socket connect error");
      }
    }
  } else {
    errif(::connect(fd_, (sockaddr *)&addr_info, sizeof(addr_info)) == -1, "socket connect error");
  }
}

void Socket::connect(const char *ip, uint16_t port) {
  InetAddr *addr = new InetAddr(ip, port);
  connect(addr);
  delete addr;
}

int Socket::get_fd() { return fd_; }
