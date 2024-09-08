/*
 * @Author: Xudong0722
 * @Date: 2024-09-07 22:36:18
 * @Last Modified by: Xudong0722
 * @Last Modified time: 2024-09-08 13:24:10
 */

#include "InetAddr.h"
#include <string.h>

InetAddr::InetAddr() : addr_len_(sizeof(addr_info_))
{
    bzero(&addr_info_, sizeof(addr_info_));
}

InetAddr::InetAddr(const char *ip, uint16_t port) : addr_len_(sizeof(addr_info_))
{
    bzero(&addr_info_, sizeof(addr_info_));
    addr_info_.sin_family = AF_INET;
    addr_info_.sin_addr.s_addr = inet_addr(ip);
    addr_info_.sin_port = htons(port);
    addr_len_ = sizeof(addr_info_);
}

InetAddr::~InetAddr() = default;