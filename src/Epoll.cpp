/*
 * @Author: Xudong0722
 * @Date: 2024-09-08 13:26:10
 * @Last Modified by: Xudong0722
 * @Last Modified time: 2024-11-10 17:18:44
 */

#include "Epoll.h"
#include "util.h"
#include "Channel.h"
#include <unistd.h>
#include <string.h>
#include <iostream>

#define MAX_EVENTS 1000

Epoll::Epoll() : epfd_(-1), events_(nullptr)
{
    epfd_ = kqueue();
    errif(epfd_ == -1, "epoll create failed.");
    events_ = new struct kevent[MAX_EVENTS];
    bzero(events_, sizeof(struct kevent) * MAX_EVENTS);
}

Epoll::~Epoll()
{
    if (epfd_ != -1)
    {
        close(epfd_);
        epfd_ = -1;
    }
    delete[] events_;
}

void Epoll::add_fd(int fd, uint32_t op, Channel *channel)
{
    struct kevent ev;
    bzero(&ev, sizeof(ev));
    EV_SET(&ev, fd, EVFILT_READ, op, 0, 0, (void *)channel);
    int r = kevent(epfd_, &ev, 1, NULL, 0, NULL);
    errif(r, "kevent failed.");
}

void Epoll::update_channel(Channel *channel)
{
    if (nullptr == channel)
        return;
    int fd = channel->get_fd();
    struct kevent ev;
    bzero(&ev, sizeof(ev));
    if (channel->in_epoll())
    {
        EV_SET(&ev, fd, EVFILT_READ, channel->get_events(), 0, 0, (void *)channel);
        int r = kevent(epfd_, &ev, 1, NULL, 0, NULL);
        errif(r, "kevent failed.");
        channel->set_in_epoll(true);
    }
    else
    {
        EV_SET(&ev, fd, EVFILT_READ, channel->get_events(), 0, 0, (void *)channel);
        int r = kevent(epfd_, &ev, 1, NULL, 0, NULL);
        errif(r, "kevent failed.");
    }
}

std::vector<Channel *> Epoll::wait(int timeout)
{
    std::vector<Channel *> active_channels;
    struct timespec time_out
    {
        timeout, 0
    };
    int n = kevent(epfd_, NULL, 0, events_, MAX_EVENTS, &time_out);
    errif(n == -1, "epoll wait error.");
    for (int i = 0; i < n; ++i)
    {
        Channel *ch = (Channel *)events_[i].udata;
        std::cout << ch << std::endl;
        ch->set_revents(events_[i].filter);
        std::cout << ch << " " << events_[i].filter << std::endl;
        active_channels.emplace_back(ch);
    }
    return active_channels;
}
