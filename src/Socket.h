/*
 * @Author: Xudong0722
 * @Date: 2024-09-08 12:45:07
 * @Last Modified by: Xudong0722
 * @Last Modified time: 2024-09-08 13:24:28
 */

#pragma once

class InetAddr;

class Socket
{
public:
    Socket();
    Socket(int _fd);
    ~Socket();

public:
    void bind(InetAddr *);
    void listen();
    void set_non_blocking();
    int accept(InetAddr *);

    int get_fd();

private:
    int fd_;
};