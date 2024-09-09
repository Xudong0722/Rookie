/*
 * @Author: Xudong0722
 * @Date: 2024-09-08 13:26:07
 * @Last Modified by: Xudong0722
 * @Last Modified time: 2024-09-09 19:11:31
 */
#pragma once
#include <vector>
#include <sys/event.h>

class Channel;

class Epoll
{
private:
    int epfd_;
    struct kevent *events;

public:
    Epoll();
    ~Epoll();

    void add_fd(int fd, uint32_t op, Channel *channel);
    void update_channel(Channel *channel);
    std::vector<Channel *> wait(int timeout = -1);
};