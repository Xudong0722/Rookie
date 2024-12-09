/*
 * @Author: Xudong0722
 * @Date: 2024-09-09 15:25:45
 * @Last Modified by: Xudong0722
 * @Last Modified time: 2024-11-12 21:09:24
 */
#include "Channel.h"
#include "EventLoop.h"
#include "Socket.h"
#include <unistd.h>
#include <sys/epoll.h>

Channel::Channel(EventLoop *event_loop, int fd)
    : event_loop_(event_loop), fd_(fd), is_use_threadpool_(true)
{
}

Channel::~Channel()
{
    if (fd_ != -1)
    {
        close(fd_);
        fd_ = -1;
    }
}

void Channel::enable_reading()
{
    events_ |= EPOLLIN | EPOLLPRI;
    event_loop_->update_channel(this);
}

void Channel::use_ET()
{
    events_ |= EPOLLET;
    event_loop_->update_channel(this);
}

void Channel::handle_event()
{
    // printf("Debug, Channel::handle_event()\n");
    // // cb_();
    // event_loop_->add_task(cb_); // we can't set non_blocking mode for socket

    if (ready_ & (EPOLLIN | EPOLLPRI))
    {
        if (is_use_threadpool_)
            event_loop_->add_task(read_cb_);
        else
            read_cb_();
    }
    else if (ready_ & EPOLLOUT)
    {
        if (is_use_threadpool_)
            event_loop_->add_task(write_cb_);
        else
            write_cb_();
    }
}

int Channel::get_fd()
{
    return fd_;
}

uint32_t Channel::get_events() const
{
    return events_;
}

void Channel::set_events(uint32_t events)
{
    events_ = events;
}

uint32_t Channel::get_ready() const
{
    return ready_;
}

void Channel::set_ready(uint32_t ready)
{
    ready_ = ready;
}

bool Channel::get_in_epoll() const
{
    return is_in_epoll_;
}

void Channel::set_in_epoll(bool is_in_epoll)
{
    is_in_epoll_ = is_in_epoll;
}

void Channel::set_read_callback(std::function<void()> cb)
{
    if (nullptr == cb)
        return;
    read_cb_ = cb;
}

void Channel::set_use_threadpool(bool use)
{
    is_use_threadpool_ = use;
}
