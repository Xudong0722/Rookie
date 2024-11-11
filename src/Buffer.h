/*
 * @Author: Xudong0722
 * @Date: 2024-11-12 00:12:58
 * @Last Modified by: Xudong0722
 * @Last Modified time: 2024-11-12 00:21:17
 */
#pragma once
#include <string>

class Buffer
{
public:
    Buffer();
    ~Buffer();
    void append(const char *str, int size);
    ssize_t size();
    const char *c_str();
    void clear();
    void getline();

private:
    std::string buf_;
};