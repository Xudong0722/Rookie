#pragma once
#include <string>

enum class LogLevel {
  DBG = 0,
  INFO = 1,
  WARN = 2,
  ERR = 3,
};

// TODO
const std::string output_file_path{"/elivs/Rookie/build/log/"};

class Elog {
 private:
  Elog();
  ~Elog() = default;

 public:
  static Elog &GetInst();
  void set_log_level(LogLevel log_level);
  void log(const LogLevel log_level, const std::string &msg);

 private:
  LogLevel log_level_;
};