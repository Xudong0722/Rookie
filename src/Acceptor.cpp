#include "Acceptor.h"
/*
 * @Author: Xudong0722
 * @Date: 2024-09-18 17:25:28
 * @Last Modified by: Xudong0722
 * @Last Modified time: 2024-09-18 17:59:32
 */

#include "Acceptor.h"
#include "Channel.h"
#include "EventLoop.h"
#include "Socket.h"
#include "InetAddr.h"

Acceptor::Acceptor(EventLoop *loop)
    : loop_(loop)
{
    addr_ = new InetAddr("127.0.0.1", 8888);
    sock_ = new Socket();

    sock_->bind(addr_);
    sock_->listen();
    sock_->set_non_blocking();
    accept_channel_ = new Channel(loop_, sock_->get_fd());

    std::function<void()> cb = std::bind(&Acceptor::accept_connection, this);
    accept_channel_->set_callback(cb);
    accept_channel_->enable_reading();
}

Acceptor::~Acceptor()
{
    delete addr_;
    delete sock_;
    delete accept_channel_;
}

void Acceptor::accept_connection()
{
    new_connection_callback_(sock_);
}

void Acceptor::set_new_connection_callback(std::function<void(Socket *)> cb)
{
    new_connection_callback_ = cb;
}
