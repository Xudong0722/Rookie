/*
 * @Author: Xudong0722 
 * @Date: 2024-12-26 23:23:25 
 * @Last Modified by: Xudong0722
 * @Last Modified time: 2024-12-27 00:30:39
 */
#pragma once
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <mutex>
#include "ETime.h"

enum class LogLevel {
  DBG = 0,
  INFO = 1,
  WARN = 2,
  ERR = 3,
};


#define LOG(level, ...) Elog::GetInst().log(LogLevel::level, __VA_ARGS__)


class Elog {
 private:
  Elog();
  ~Elog();

 public:
  static Elog &GetInst();
  void set_log_level(LogLevel log_level);
  void set_log_file(const std::string& file_name);
  
  template<typename... Args>
  void log(const LogLevel log_level, Args... args);
  std::string get_log_level(LogLevel log_level);
 private:
  LogLevel log_level_;
  std::ofstream log_file_;
  std::mutex log_mutex_;
};

template<typename... Args>
void Elog::log(const LogLevel log_level, Args... args)
{
  if (log_level < this->log_level_) return;
  std::ostringstream oss;

  oss << "[" << ETime::GetInst().get_cur_time() << "] ";
  oss << "[" << get_log_level(log_level) << "] ";
  (oss << ... << args);
  oss << std::endl;
  {
    std::unique_lock<std::mutex> lock(log_mutex_);
    if(log_file_.is_open()){
      log_file_ << oss.str();
      log_file_.flush();
      std::cerr << "log file is open\n";
      if(log_file_.fail()){
        std::cerr << "Failed to write to log file\n";
      }
    }
  }

  std::cout << oss.str() << std::endl;
}