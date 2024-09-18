/*
 * @Author: Xudong0722 
 * @Date: 2024-09-09 23:00:21 
 * @Last Modified by: Xudong0722
 * @Last Modified time: 2024-09-18 18:01:44
 */

#include "Server.h"
#include "EventLoop.h"
#include "Socket.h"
#include "InetAddr.h"
#include "Channel.h"
#include "Acceptor.h"
#include <functional>
#include <unistd.h>

Server::Server(EventLoop *event_loop)
: event_loop_(event_loop)
{
    Socket *server_sock = new Socket;
    InetAddr *server_addr = new InetAddr("127.0.0.1", 8888);

    server_sock->bind(server_addr);
    server_sock->listen();
    server_sock->set_non_blocking();

    Channel *server_channel = new Channel(event_loop, server_sock->get_fd());
    std::function<void()> cb = std::bind(&Server::handle_new_connect_event, this, server_sock);
    server_channel->set_callback(cb);
    server_channel->enable_reading();
}

Server::~Server()
{
    
}

void Server::handle_read_event(int sockfd)
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
            close(sockfd);
            break;
        }
    }
}

void Server::handle_new_connect_event(Socket *server_sock)
{
    InetAddr *new_client_addr = new InetAddr;
    Socket *new_client_sock = new Socket(server_sock->accept(new_client_addr));

    printf("new client fd %d! IP: %s Port: %d\n", new_client_sock->get_fd(), inet_ntoa(new_client_addr->addr_info_.sin_addr), ntohs(new_client_addr->addr_info_.sin_port));
    new_client_sock->set_non_blocking();
    Channel *new_client_channel = new Channel(event_loop_, new_client_sock->get_fd());
    std::function<void()> cb = std::bind(&Server::handle_read_event, this, new_client_sock->get_fd());
    new_client_channel->set_callback(cb);
    new_client_channel->enable_reading();
}
