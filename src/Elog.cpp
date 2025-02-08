/*
 * @Author: Xudong0722 
 * @Date: 2024-12-26 23:35:16 
 * @Last Modified by: Xudong0722
 * @Last Modified time: 2024-12-27 00:29:39
 */
#include "Elog.h"
#include "ETime.h"
#include <iostream>

Elog::Elog() : log_level_(LogLevel::INFO) {}

Elog::~Elog()
{
  if(log_file_.is_open()){
    log_file_.flush();
    log_file_.close();
  }
}

Elog &Elog::GetInst() {
  static Elog logger;
  return logger;
}

void Elog::set_log_level(const LogLevel log_level) { log_level_ = log_level; }

void Elog::set_log_file(const std::string& file_name)
{
  if(file_name.empty()) return ;

  std::unique_lock<std::mutex> lock(log_mutex_);
  if(log_file_.is_open()){
    log_file_.close();
  }

  log_file_.open(file_name, std::ios::out | std::ios::app);
  if(!log_file_.is_open()){
    std::cerr << "file open failed: " << file_name << '\n';
  }
}

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