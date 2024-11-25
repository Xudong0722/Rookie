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
    : ep_(nullptr), thread_pool_(nullptr), quit_(false)
{
    ep_ = new Epoll();
    thread_pool_ = new ThreadPool();
}

EventLoop::~EventLoop()
{
    delete ep_;
    delete thread_pool_;
}

void EventLoop::loop()
{
    while (!quit_)
    {
        auto active_channels = ep_->wait(30);
        std::cout << "DEBUG " << active_channels.size() << std::endl;
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

void EventLoop::add_task(std::function<void()> task)
{
    if (nullptr == thread_pool_)
        return;

    thread_pool_->add(task);
}
