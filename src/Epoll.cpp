/*
 * @Author: Xudong0722
 * @Date: 2024-09-08 13:26:10
 * @Last Modified by: Xudong0722
 * @Last Modified time: 2024-09-08 22:18:01
 */

#include "Epoll.h"
#include "util.h"
#include <unistd.h>
#include <string.h>

#define MAX_EVENTS 1000

Epoll::Epoll() : epfd_(-1), events(nullptr)
{
    epfd_ = kqueue();
    errif(epfd_ == -1, "epoll create failed.");
    events = new struct kevent[MAX_EVENTS];
    bzero(events, sizeof(struct kevent) * MAX_EVENTS);
}

Epoll::~Epoll()
{
    if (epfd_ != -1)
    {
        close(epfd_);
        epfd_ = -1;
    }
    delete[] events;
}

void Epoll::add_fd(int fd, uint32_t op)
{
    struct kevent ev;
    bzero(&ev, sizeof(ev));
    EV_SET(&ev, fd, EVFILT_READ, op, 0, 0, (void *)(intptr_t)fd);
    int r = kevent(epfd_, &ev, 1, NULL, 0, NULL);
    errif(r, "kevent failed.");
}

std::vector<struct kevent> Epoll::wait(int timeout)
{
    std::vector<struct kevent> active_events;
    struct timespec time_out
    {
        timeout, 0
    };
    int n = kevent(epfd_, NULL, 0, events, MAX_EVENTS, &time_out);
    errif(n == -1, "epoll wait error.");
    for (int i = 0; i < n; ++i)
    {
        active_events.emplace_back(events[i]);
    }
    return active_events;
}
