#include "Connection.h"
/*
 * @Author: Xudong0722
 * @Date: 2024-09-18 19:44:49
 * @Last Modified by: Xudong0722
 * @Last Modified time: 2024-11-12 00:34:55
 */
#include "Connection.h"
#include "EventLoop.h"
#include "Socket.h"
#include "Channel.h"
#include "Buffer.h"
#include "util.h"
#include <string.h>
#include <unistd.h>
#include <iostream>

#define READ_BUFFER 1024

Connection::Connection(EventLoop *loop, Socket *sock)
    : event_loop_(loop), sock_(sock)
{
    channel_ = new Channel(loop, sock_->get_fd());
    std::function<void()> cb = std::bind(&Connection::echo, this, sock_->get_fd());
    channel_->set_callback(cb);
    channel_->enable_reading();
    read_buffer_ = new Buffer();
}

Connection::~Connection()
{
    delete channel_;
    delete sock_;
    delete read_buffer_;
}

void Connection::echo(int sockfd)
{
    char buf[READ_BUFFER];
    while (true)
    {
        bzero(&buf, sizeof(buf));
        ssize_t bytes_read = read(sockfd, buf, sizeof(buf));
        std::cout << __PRETTY_FUNCTION__ << " " << bytes_read << std::endl;
        if (bytes_read > 0)
        {
            printf("message from client fd: %d, message: %s\n", sockfd, buf);
            // write(sockfd, buf, sizeof(buf));
            read_buffer_->append(buf, bytes_read);
        }
        else if (bytes_read == -1 && errno == EINTR)
        {
            printf("continue reading.");
            continue;
        }
        else if (bytes_read == -1 && ((errno == EAGAIN) || (errno == EWOULDBLOCK)))
        {
            printf("finish reading once, errno: %d\n", errno);
            printf("message from client fd %d: %s\n", sockfd, read_buffer_->c_str());
            errif(write(sockfd, read_buffer_->c_str(), read_buffer_->size()) == -1, "socket write error");
            read_buffer_->clear();
            break;
        }
        else if (bytes_read == 0)
        {
            printf("EOF, client fd %d disconnected\n", sockfd);
            // close(sockfd);
            delete_connection_callback_(sock_);
            break;
        }
    }
}

void Connection::set_delete_connection_callback(std::function<void(Socket *)> cb)
{
    delete_connection_callback_ = cb;
}
