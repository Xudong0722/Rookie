/*
 * @Author: Xudong0722
 * @Date: 2024-09-09 15:20:27
 * @Last Modified by: Xudong0722
 * @Last Modified time: 2024-09-09 23:09:00
 */

#pragma once
#include <sys/event.h>
#include <functional>

class Epoll;
class EventLoop;

class Channel
{
public:
    Channel(EventLoop *event_loop, int fd);
    ~Channel();

    void enable_reading();
    void handle_event();

    int get_fd();
    uint32_t get_events() const;
    void set_revents(uint32_t revents);
    uint32_t get_revents() const;

    bool in_epoll() const;
    void set_in_epoll(bool is_in_epoll);

    void set_callback(std::function<void()> cb);

private:
    EventLoop *event_loop_;
    int fd_{-1};

    uint32_t events_{0};
    uint32_t revents_{0};
    bool is_in_epoll_{false};
    std::function<void()> cb_;
};