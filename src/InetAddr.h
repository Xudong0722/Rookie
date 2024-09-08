/*
 * @Author: Xudong0722
 * @Date: 2024-09-07 22:33:36
 * @Last Modified by: Xudong0722
 * @Last Modified time: 2024-09-08 13:24:12
 */
#pragma once
#include <arpa/inet.h>

class InetAddr
{
public:
    InetAddr();
    InetAddr(const char *ip, uint16_t port);
    ~InetAddr() = default;

public:
    struct sockaddr_in addr_info_;
    socklen_t addr_len_;
};