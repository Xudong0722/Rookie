/*
 * @Author: Xudong0722
 * @Date: 2024-09-09 22:31:33
 * @Last Modified by: Xudong0722
 * @Last Modified time: 2024-10-28 22:44:28
 */

#pragma once
#include <functional>

class Epoll;
class Channel;
class ThreadPool;

class EventLoop
{
public:
    EventLoop();
    ~EventLoop();

    void loop();
    void update_channel(Channel *channel);
    void add_task(std::function<void()> task);

private:
    Epoll *ep_;
    ThreadPool *thread_pool_;
    bool quit_{false};
};
