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

#define MAX_EVENTS 1000

Epoll::Epoll() : epfd_(-1), events_(nullptr)
{
    epfd_ = epoll_create1(0);
    errif(epfd_ == -1, "epoll create failed.");
    events_ = new epoll_event[MAX_EVENTS];
    bzero(events_, sizeof(*events_) * MAX_EVENTS);
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

// void Epoll::add_fd(int fd, uint32_t op, Channel *channel)
// {
//     struct kevent ev;
//     bzero(&ev, sizeof(ev));
//     EV_SET(&ev, fd, EVFILT_READ, op, 0, 0, (void *)channel);
//     int r = kevent(epfd_, &ev, 1, NULL, 0, NULL);
//     errif(r, "kevent failed.");
// }

void Epoll::update_channel(Channel *channel)
{
    if (nullptr == channel)
        return;
    int fd = channel->get_fd();
    struct epoll_event ev;
    bzero(&ev, sizeof(ev));
    ev.data.ptr = channel;
    ev.events = channel->get_events();
    if (!channel->get_in_epoll())
    {
        errif(epoll_ctl(epfd_, EPOLL_CTL_ADD, fd, &ev) == -1, "epoll add error");
        channel->set_in_epoll(true);
    }
    else
    {
        errif(epoll_ctl(epfd_, EPOLL_CTL_MOD, fd, &ev) == -1, "epoll modify error");
    }
}

std::vector<Channel *> Epoll::wait(int timeout)
{
    std::vector<Channel *> active_channels;

    int n = epoll_wait(epfd_, events_, MAX_EVENTS, timeout);
    errif(n == -1, "epoll wait error.");
    for (int i = 0; i < n; ++i)
    {
        Channel *ch = (Channel *)events_[i].data.ptr;
        ch->set_ready(events_[i].events);
        active_channels.emplace_back(ch);
    }
    return active_channels;
}
