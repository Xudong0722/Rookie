/*
 * @Author: Xudong0722
 * @Date: 2024-09-09 23:00:21
 * @Last Modified by: Xudong0722
 * @Last Modified time: 2024-09-18 22:58:13
 */

#include "Server.h"
#include "EventLoop.h"
#include "Socket.h"
#include "InetAddr.h"
#include "Channel.h"
#include "Acceptor.h"
#include "Connection.h"
#include <functional>
#include <unistd.h>

Server::Server(EventLoop *event_loop)
    : event_loop_(event_loop)
{
    acceptor_ = new Acceptor(event_loop_);
    std::function<void(Socket *)> cb = std::bind(&Server::handle_new_connect_event, this, std::placeholders::_1);
    acceptor_->set_new_connection_callback(cb);
}

Server::~Server()
{
    delete acceptor_;
}

void Server::handle_new_connect_event(Socket *sock)
{
    if (nullptr == sock)
        return;
    Connection *conn = new Connection(event_loop_, sock);
    std::function<void(Socket *)> cb = std::bind(&Server::delete_connection, this, std::placeholders::_1);
    conn->set_delete_connection_callback(cb);
    connections_[sock->get_fd()] = conn;
}

void Server::delete_connection(Socket *sock)
{
    if (nullptr == sock)
        return;
    close(sock->get_fd());
    auto tmp = connections_[sock->get_fd()];
    connections_.erase(sock->get_fd());
    delete tmp;
}