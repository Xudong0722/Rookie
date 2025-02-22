/*
 * @Author: Xudong0722 
 * @Date: 2025-02-22 16:12:22 
 * @Last Modified by: Xudong0722
 * @Last Modified time: 2025-02-22 17:16:13
 */

#include "Env.h"
#include "Elog.h"
#include "ETime.h"

#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string>
#include <sstream>
#include <thread>
#include <cstdlib>
#include <cassert>

Env::Env()
{
    std::string log_file_name = ETime::GetInst().get_cur_time();
    std::ostringstream thread_stream;
    thread_stream << std::this_thread::get_id();
    log_file_name += thread_stream.str();
    
    if(::mkdir(log_file_name.c_str(), 0755) == 0){
        log_file_name = log_file_name + "//" + log_file_name + "LOG";
        int fd = ::open(log_file_name.c_str(), O_APPEND | O_WRONLY | O_CREAT, 0644);
        if(fd >= 0){
            FILE* fp = ::fdopen(fd, "w");
            if(nullptr != fp){
                logger_ = std::make_unique<Elog>(fp);
            }else{
                ::close(fd);
            }
        }

    }
}

Env& Env::GetInst()
{
    static Env env;
    return env;
}

Elog* Env::get_logger()
{
    return logger_.get();
}

void Env::Log(const char* format, ...)
{
  assert(get_logger() != nullptr);
  std::va_list ap;
  va_start(ap, format);
  get_logger()->Log(format, ap);
  va_end(ap);
}