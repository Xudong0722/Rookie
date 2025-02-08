/*
 * @Author: Xudong0722
 * @Date: 2024-11-12 00:12:54
 * @Last Modified by: Xudong0722
 * @Last Modified time: 2025-02-08 18:37:30
 */
#include "Buffer.h"
#include <iostream>

Buffer::Buffer() {}

Buffer::~Buffer() {}

void Buffer::append(const char *str, int size) {
  if (nullptr == str) return;
  for (int i = 0; i < size; ++i) {
    if (str[i] == '\0') break;
    buf_.push_back(str[i]);
  }
}

ssize_t Buffer::size() { return buf_.size(); }

const char *Buffer::c_str() { return buf_.c_str(); }

void Buffer::clear() { buf_.clear(); }

void Buffer::getline() {
  clear();
  std::getline(std::cin, buf_);
}

void Buffer::set_buf(const char *buf) {
  buf_.clear();
  buf_.append(buf);
}
