/*
 * @Author: Xudong0722 
 * @Date: 2024-12-25 23:00:38 
 * @Last Modified by: Xudong0722
 * @Last Modified time: 2024-12-25 23:02:21
 */
#pragma once

#include <chrono>
#include <string>

class ETime{
private:
    ETime() = default;
    ~ETime() = default;

public:
    static ETime& GetInst();
    std::string get_cur_time();
};
