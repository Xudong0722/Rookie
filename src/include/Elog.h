/*
 * @Author: Xudong0722 
 * @Date: 2024-12-26 23:23:25 
 * @Last Modified by: Xudong0722
 * @Last Modified time: 2025-02-22 16:51:44
 */
#pragma once
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <mutex>
#include <cstdarg>
#include "ETime.h"
#include "util.h"

enum class LogLevel {
  DBG = 0,
  INFO = 1,
  WARN = 2,
  ERR = 3,
};


//#define LOG(level, ...) Elog::GetInst().log(LogLevel::level, __VA_ARGS__)

class Elog: public noncopymoveable{
public:
  Elog(std::FILE* fp);
  ~Elog();

 public:
  void set_log_level(LogLevel log_level);
  
  // template<typename... Args>
  // void log(const LogLevel log_level, Args... args);
  std::string get_log_level(LogLevel log_level);

  void Log(const char* format, std::va_list ap);
 private:
  LogLevel log_level_;
  std::mutex log_mutex_;
  std::FILE* fp_;
};

// template<typename... Args>
// void Elog::log(const LogLevel log_level, Args... args)
// {
//   if (log_level < this->log_level_) return;
//   std::ostringstream oss;

//   oss << "[" << ETime::GetInst().get_cur_time() << "] ";
//   oss << "[" << get_log_level(log_level) << "] ";
//   (oss << ... << args);
//   oss << std::endl;
//   {
//     std::unique_lock<std::mutex> lock(log_mutex_);
//     if(log_file_.is_open()){
//       log_file_ << oss.str();
//       log_file_.flush();
//       std::cerr << "log file is open\n";
//       if(log_file_.fail()){
//         std::cerr << "Failed to write to log file\n";
//       }
//     }
//   }

//   std::cout << oss.str() << std::endl;
// }