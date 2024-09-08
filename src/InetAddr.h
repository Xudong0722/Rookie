/*
 * @Author: Xudong0722
 * @Date: 2024-09-07 22:33:36
 * @Last Modified by: Xudong0722
 * @Last Modified time: 2024-09-08 22:59:11
 */
#pragma once
#include <arpa/inet.h>

class InetAddr
{
public:
    InetAddr();
    InetAddr(const char *ip, uint16_t port);
    ~InetAddr();

public:
    struct sockaddr_in addr_info_;
    socklen_t addr_len_;
};