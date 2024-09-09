/*
 * @Author: Xudong0722
 * @Date: 2024-09-09 22:31:33
 * @Last Modified by: Xudong0722
 * @Last Modified time: 2024-09-09 22:38:40
 */

#pragma once

class Epoll;
class Channel;

class EventLoop
{
public:
    EventLoop();
    ~EventLoop();

    void loop();
    void update_channel(Channel *channel);

private:
    Epoll *ep_;
    bool quit_{false};
};
