/*
 * @Author: Xudong0722
 * @Date: 2024-09-18 19:44:52
 * @Last Modified by: Xudong0722
 * @Last Modified time: 2024-09-18 20:10:40
 */

#pragma once
#include <functional>

class EventLoop;
class Channel;
class Socket;

class Connection
{
public:
    Connection(EventLoop *loop, Socket *sock);
    ~Connection();

    void echo(int sockfd);
    void set_delete_connection_callback(std::function<void(Socket *)>);

private:
    EventLoop *event_loop_;
    Socket *sock_;
    Channel *channel_;
    std::function<void(Socket *)> delete_connection_callback_;
};