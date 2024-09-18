#include "Connection.h"
/*
 * @Author: Xudong0722
 * @Date: 2024-09-18 19:44:49
 * @Last Modified by: Xudong0722
 * @Last Modified time: 2024-09-18 20:27:50
 */
#include "Connection.h"
#include "EventLoop.h"
#include "Socket.h"
#include "Channel.h"
#include <string.h>
#include <unistd.h>

#define READ_BUFFER 1024

Connection::Connection(EventLoop *loop, Socket *sock)
    : event_loop_(loop), sock_(sock)
{
    channel_ = new Channel(loop, sock_->get_fd());
    std::function<void()> cb = std::bind(&Connection::echo, this, sock_->get_fd());
    channel_->set_callback(cb);
    channel_->enable_reading();
}

Connection::~Connection()
{
    delete channel_;
    delete sock_;
}

void Connection::echo(int sockfd)
{
    char buf[READ_BUFFER];
    while (true)
    {
        bzero(&buf, sizeof(buf));
        ssize_t bytes_read = read(sockfd, buf, sizeof(buf));

        if (bytes_read > 0)
        {
            printf("message from client fd: %d, message: %s\n", sockfd, buf);
            write(sockfd, buf, sizeof(buf));
        }
        else if (bytes_read == -1 && errno == EINTR)
        {
            printf("continue reading.");
            continue;
        }
        else if (bytes_read == -1 && ((errno == EAGAIN) || (errno == EWOULDBLOCK)))
        {
            printf("finish reading once, errno: %d\n", errno);
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
