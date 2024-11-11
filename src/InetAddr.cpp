/*
 * @Author: Xudong0722
 * @Date: 2024-09-07 22:36:18
 * @Last Modified by: Xudong0722
 * @Last Modified time: 2024-11-12 00:57:51
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

void InetAddr::set_inet_addr(sockaddr_in addr, socklen_t addr_len)
{
    addr_info_ = addr;
    addr_len_ = addr_len;
}

sockaddr_in InetAddr::get_addr()
{
    return addr_info_;
}

socklen_t InetAddr::get_addr_len()
{
    return addr_len_;
}
