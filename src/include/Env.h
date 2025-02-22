/*
 * @Author: Xudong0722 
 * @Date: 2025-02-22 15:56:21 
 * @Last Modified by: Xudong0722
 * @Last Modified time: 2025-02-22 16:56:53
 */

#pragma once
#include <memory>
#include "util.h"

class Elog;

class Env: public noncopymoveable
{
private:
    Env();
    ~Env() = default;    
public:
    static Env& GetInst();
    Elog* get_logger();
    void Log(const char* format, ...)
#if defined(__GUNC__) || defined(__clang__)
__attribute__((__format__(__printf__, 2, 3)))
#endif
;
private:
    std::unique_ptr<Elog> logger_;
};