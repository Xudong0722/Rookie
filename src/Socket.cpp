/*
 * @Author: Xudong0722
 * @Date: 2024-09-08 12:49:44
 * @Last Modified by: Xudong0722
 * @Last Modified time: 2024-11-12 01:23:01
 */

#include "Socket.h"
#include "InetAddr.h"
#include "util.h"
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <string.h>

Socket::Socket() : fd_(-1)
{
    fd_ = socket(AF_INET, SOCK_STREAM, 0);
    errif(fd_ == -1, "Create socket failed.");
}

Socket::Socket(int _fd) : fd_(_fd)
{
    errif(fd_ == -1, "Create socket failed.");
}

Socket::~Socket()
{
    if (fd_ != -1)
    {
        close(fd_);
        fd_ = -1;
    }
}

void Socket::bind(InetAddr *addr_info)
{
    errif(NULL == addr_info, "addr_info is nullptr");
    errif(::bind(fd_, (sockaddr *)addr_info, addr_info->addr_len_) == -1, "bind socket failed.");
}

void Socket::listen()
{
    errif(::listen(fd_, SOMAXCONN) == -1, "Socket listen failed.");
}

void Socket::set_non_blocking()
{
    fcntl(fd_, F_SETFL, fcntl(fd_, F_GETFL) | O_NONBLOCK);
}

int Socket::accept(InetAddr *addr_info)
{
    struct sockaddr_in addr;
    socklen_t addr_len = sizeof(addr);
    bzero(&addr, sizeof(addr));
    errif(NULL == addr_info, "addr_info is nullptr.");
    int new_client_sockfd = ::accept(fd_, (sockaddr *)&addr, &addr_len);
    errif(new_client_sockfd == -1, "Socket accept error....");
    addr_info->set_inet_addr(addr, addr_len);
    return new_client_sockfd;
}

void Socket::connect(InetAddr *addr)
{
    struct sockaddr_in addr_info = addr->get_addr();
    socklen_t addr_len = addr->get_addr_len();
    errif(::connect(fd_, (sockaddr *)&addr_info, addr_len) == -1, "socket connect error.");
}

int Socket::get_fd()
{
    return fd_;
}
