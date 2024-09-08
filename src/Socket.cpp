/*
 * @Author: Xudong0722
 * @Date: 2024-09-08 12:49:44
 * @Last Modified by: Xudong0722
 * @Last Modified time: 2024-09-08 13:25:28
 */

#include "Socket.h"
#include "InetAddr.h"
#include "util.h"
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>

Socket::Socket() : fd_(-1)
{
    fd_ = socket(AF_INET, SOCK_STREAM, 0);
    errif(fd_ == -1, "Create socket failed.");
}

Socket::Socket(int _fd) : fd_(_fd)
{
    errif(fd_ == -1, "Create socket failed.");
}

Socket::~Socket() = default;

void Socket::bind(InetAddr *addr_info)
{
    errif(NULL == addr_info, "addr_info is nullptr");
    errif(::bind(fd_, (sockaddr *)addr_info, sizeof(addr_info)) == -1, "bind socket failed.");
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
    errif(NULL == addr_info, "addr_info is nullptr.");
    int new_client_sockfd = ::accept(fd_, (sockaddr *)&addr_info->addr_info_, &addr_info->addr_len_);
    errif(new_client_sockfd == -1, "Socket accept error.");
    return new_client_sockfd;
}

int Socket::get_fd()
{
    return fd_;
}
