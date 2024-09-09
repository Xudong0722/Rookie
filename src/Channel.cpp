/*
 * @Author: Xudong0722
 * @Date: 2024-09-09 15:25:45
 * @Last Modified by: Xudong0722
 * @Last Modified time: 2024-09-09 23:20:06
 */
#include "Channel.h"
#include "Epoll.h"
#include "EventLoop.h"

Channel::Channel(EventLoop *event_loop, int fd)
    : event_loop_(event_loop), fd_(fd)
{
}

Channel::~Channel()
{
}

void Channel::enable_reading()
{
    events_ = EV_ADD | EV_ENABLE;
    event_loop_->update_channel(this);
}

void Channel::handle_event()
{
    cb_();
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

void Channel::set_callback(std::function<void()> cb)
{
    if (nullptr == cb)
        return;
    cb_ = cb;
}
