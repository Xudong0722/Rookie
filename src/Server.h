/*
 * @Author: Xudong0722
 * @Date: 2024-09-09 22:31:18
 * @Last Modified by: Xudong0722
 * @Last Modified time: 2024-09-18 22:42:11
 */
#pragma once

#include <map>

#define READ_BUFFER 1024

class EventLoop;
class Socket;
class Acceptor;
class Connection;

class Server
{
public:
    Server(EventLoop *event_loop);
    ~Server();

    void handle_new_connect_event(Socket *server_sock);
    void delete_connection(Socket *sock);

private:
    EventLoop *event_loop_{nullptr};
    Acceptor *acceptor_{nullptr};
    std::map<int, Connection *> connections_;
};