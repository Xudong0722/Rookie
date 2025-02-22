/*
 * @Author: Xudong0722 
 * @Date: 2024-12-26 23:35:16 
 * @Last Modified by: Xudong0722
 * @Last Modified time: 2025-02-23 02:23:58
 */
#include "Elog.h"
#include "ETime.h"
#include <iostream>
#include <cstdarg>
#include <cassert>
#include <thread>
#include <sys/time.h>

//Elog takes ownership of the file handle.
Elog::Elog(std::FILE* fp) : fp_(fp), log_level_(LogLevel::INFO) {}

Elog::~Elog()
{
  std::fclose(fp_);
}

// Elog &Elog::GetInst() {
//   static Elog logger;
//   return logger;
// }

void Elog::set_log_level(const LogLevel log_level) { log_level_ = log_level; }

// void Elog::set_log_file(const std::string& file_name)
// {
//   if(file_name.empty()) return ;

//   std::unique_lock<std::mutex> lock(log_mutex_);
//   if(log_file_.is_open()){
//     log_file_.close();
//   }

//   log_file_.open(file_name, std::ios::out | std::ios::app);
//   if(!log_file_.is_open()){
//     std::cerr << "file open failed: " << file_name << '\n';
//   }
// }

std::string Elog::get_log_level(LogLevel log_level)
{
  switch (log_level)
  {
  case LogLevel::DBG:
    return "DBG";
  case LogLevel::INFO:
    return "INFO";
  case LogLevel::WARN:
    return "WARN";
  case LogLevel::ERR:
    return "ERR";
  default:
    break;
  }
  return "";
}

void Elog::Log(const char* format, std::va_list ap)
{
  //Curent time
  struct ::timeval now_timeval;
  ::gettimeofday(&now_timeval, nullptr);
  const std::time_t now_seconds = now_timeval.tv_sec;
  struct std::tm now_components;
  ::localtime_r(&now_seconds, &now_components);

  //Thread id
  constexpr const int MAX_THREAD_ID_SIZE = 32;
  std::ostringstream thread_stream;
  thread_stream << std::this_thread::get_id();
  std::string thread_id = thread_stream.str();
  if(thread_id.size() > MAX_THREAD_ID_SIZE){
    thread_id.resize(MAX_THREAD_ID_SIZE);
  }

  //First time, we attempt to print info into a stack-allocated buffer, if failed, we make a second attempt with a dynamically allocated buffer.
  constexpr const int STACK_MAX_BUFFER_SIZE = 512;
  char stack_buffer[STACK_MAX_BUFFER_SIZE]{};
  static_assert(sizeof(stack_buffer) == static_cast<size_t>(STACK_MAX_BUFFER_SIZE), "sizeof(char) is expected to be 1 in cpp.");
  int dynamic_buffer_size{0};

  for(int iteration = 0; iteration < 2; ++iteration){
    char* const buffer = (iteration == 0) ? stack_buffer : new char[dynamic_buffer_size];
    const int buffer_size = (iteration == 0) ? STACK_MAX_BUFFER_SIZE : dynamic_buffer_size;

    //Firstly, print time and thread id
    //Ref:https://man7.org/linux/man-pages/man3/snprintf.3p.html
    int buffer_offset = snprintf(buffer, buffer_size, 
      "%04d/%02d/%02d-%02d:%02d:%02d.%06d %s ",
      now_components.tm_year + 1900, now_components.tm_mon + 1,
      now_components.tm_mday, now_components.tm_hour, now_components.tm_min,
      now_components.tm_sec, static_cast<int>(now_timeval.tv_usec),
      thread_id.c_str()
    );

    //The header(time + thread id) can be at most 28 characters(10 date, 15 time and 3 delimiters) plus the thread id
    //which should fit comfortably into the static buffer.
    assert(buffer_offset <= 28 + MAX_THREAD_ID_SIZE);
    static_assert(28 + MAX_THREAD_ID_SIZE < STACK_MAX_BUFFER_SIZE, "The stack-allocated buffer may not fit the header.");
    assert(buffer_offset < buffer_size);

    std::va_list args_copy;
    va_copy(args_copy, ap);
    buffer_offset += std::vsnprintf(buffer + buffer_offset, buffer_size - buffer_offset,
                                    format, args_copy);
    va_end(args_copy);

    //We need to insert a null terminator and a new line
    if(buffer_offset >= buffer_size - 1){
      if(iteration == 0){
        dynamic_buffer_size = buffer_offset + 2;
        continue;
      }
    }

    if(buffer[buffer_offset - 1] != '\n'){
      buffer[buffer_offset] = '\n';
      ++buffer_offset;
    }
    
    assert(buffer_offset <= buffer_size);
    std::fwrite(buffer, 1, buffer_offset, fp_);
    std::fflush(fp_);

    if(iteration != 0){
      delete []buffer;
    }
    break;
  }

}