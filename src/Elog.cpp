#include "Elog.h"
#include "ETime.h"
#include <fstream>
#include <iostream>
#include <sstream>

Elog::Elog() : log_level_(LogLevel::INFO) {}

Elog &Elog::GetInst() {
  static Elog logger;
  return logger;
}

void Elog::set_log_level(const LogLevel log_level) { log_level_ = log_level; }

void Elog::log(const LogLevel log_level, const std::string &msg) {
  if (log_level < this->log_level_) return;

  std::string file_name{"test_log.txt"};
  // std::string file_path = output_file_path + file_name;  //TODO
  std::string file_path = file_name;
  std::ofstream out_stream(file_path, std::ios::out | std::ios::app);

  if (!out_stream.is_open()) {
    std::cout << "err, file can't open\n";
    return;
  }

  out_stream << ETime::GetInst().get_cur_time() << " ";
  
  switch (log_level) {
    case LogLevel::DBG: {
      out_stream << "DBG: " << msg << '\n';
      break;
    }
    case LogLevel::INFO: {
      out_stream << "INFO: " << msg << '\n';
      break;
    }
    case LogLevel::WARN: {
      out_stream << "WARN: " << msg << '\n';
      break;
    }
    case LogLevel::ERR: {
      out_stream << "ERR: " << msg << '\n';
      break;
    }
  }
}
