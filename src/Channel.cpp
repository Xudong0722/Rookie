/*
 * @Author: Xudong0722
 * @Date: 2024-09-09 15:25:45
 * @Last Modified by: Xudong0722
 * @Last Modified time: 2024-09-09 17:25:22
 */
#include "Channel.h"
#include "Epoll.h"

Channel::Channel(Epoll *ep, int fd)
    : ep_(ep), fd_(fd)
{
}

Channel::~Channel()
{
}

void Channel::enable_reading()
{
    events_ = EV_ADD | EV_ENABLE;
    ep_->update_channel(this);
}

int Channel::get_fd()
{
    return fd_;
}

uint32_t Channel::get_events() const
{
    return events_;
}

void Channel::set_revents(uint32_t revents)
{
    revents_ = revents;
}

uint32_t Channel::get_revents() const
{
    return revents_;
}

bool Channel::in_epoll() const
{
    return is_in_epoll_;
}

void Channel::set_in_epoll(bool is_in_epoll)
{
    is_in_epoll_ = is_in_epoll;
}
