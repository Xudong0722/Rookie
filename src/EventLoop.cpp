/*
 * @Author: Xudong0722
 * @Date: 2024-09-09 22:31:36
 * @Last Modified by: Xudong0722
 * @Last Modified time: 2024-10-28 18:19:57
 */

#include "EventLoop.h"
#include "Channel.h"
#include "Epoll.h"
#include "ThreadPool.h"
#include <vector>
#include <iostream>

EventLoop::EventLoop()
    : ep_(nullptr), quit_(false)
{
    ep_ = new Epoll();
}

EventLoop::~EventLoop()
{
    delete ep_;
}

void EventLoop::loop()
{
    while (!quit_)
    {
        auto active_channels = ep_->wait(30);
        for (const auto cit : active_channels)
        {
            cit->handle_event();
        }
    }
}

void EventLoop::update_channel(Channel *channel)
{
    if (nullptr == channel)
        return;
    ep_->update_channel(channel);
}
