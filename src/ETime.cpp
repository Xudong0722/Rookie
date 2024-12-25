/*
 * @Author: Xudong0722 
 * @Date: 2024-12-25 23:02:50 
 * @Last Modified by: Xudong0722
 * @Last Modified time: 2024-12-25 23:16:33
 */


#include "ETime.h"
#include <ctime>
#include <iomanip>
#include <sstream>

ETime& ETime::GetInst()
{
    static ETime e_time;
    return e_time;
}

std::string ETime::get_cur_time()
{
    auto now = std::chrono::system_clock::now();

    auto cur_time = std::chrono::system_clock::to_time_t(now);

    std::tm* local_time = std::localtime(&cur_time);

    std::ostringstream oss;

    oss << std::put_time(local_time, "%Y-%m-%d %H:%M:%S");
    
    //append millsecond

    auto milliseconds = std::chrono::system_clock::now().time_since_epoch() % 1000;
    
    oss << "." << std::setfill('0')  << std::setw(3)  << milliseconds.count();

    return oss.str();
}