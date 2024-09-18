/*
 * @Author: Xudong0722
 * @Date: 2024-09-09 22:31:18
 * @Last Modified by: Xudong0722
 * @Last Modified time: 2024-09-18 18:01:24
 */
#pragma once

#define READ_BUFFER 1024

class EventLoop;
class Socket;
class Acceptor;

class Server
{
public:
    Server(EventLoop *event_loop);
    ~Server();

    void handle_read_event(int sockfd);
    void handle_new_connect_event(Socket *server_sock);

private:
    EventLoop *event_loop_;
    Acceptor *acceptor_;
};