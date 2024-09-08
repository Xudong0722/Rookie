/*
 * @Author: Xudong0722
 * @Date: 2024-09-08 13:26:07
 * @Last Modified by: Xudong0722
 * @Last Modified time: 2024-09-08 14:04:31
 */
#pragma once
#include <vector>
#include <sys/event.h>

class Epoll
{
private:
    int epfd_;
    struct kevent *events;

public:
    Epoll();
    ~Epoll();

    void add_fd(int fd, uint32_t op);
    std::vector<struct kevent> wait(int timeout = -1);
};