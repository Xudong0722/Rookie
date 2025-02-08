/*
 * @Author: Xudong0722 
 * @Date: 2024-12-29 13:13:58 
 * @Last Modified by:   Xudong0722 
 * @Last Modified time: 2024-12-29 13:13:58 
 */

#include <chrono>
#include <iostream>
#include <string>
#include "ThreadPool.h"

void print(int a, double b, const char *c, std::string d) { std::cout << a << b << c << d << std::endl; }

void test() { std::cout << "hellp" << std::endl; }

int sum(int a, int b) {
  std::cout << "I will sleep 5 secs.\n";
  std::this_thread::sleep_for(std::chrono::seconds(5));
  return a + b;
}

int main() {
  ThreadPool *poll = new ThreadPool();

  std::function<void()> func = std::bind(print, 1, 3.14, "hello", std::string("world"));
  auto t = poll->add(sum, 3, 4);
  poll->add(func);
  func = test;
  poll->add(func);
  // std::function<int(int, int)> func_sum = std::bind(sum, std::placeholders::_1, std::placeholders::_2);
  std::cout << t.get() << std::endl;  // will block current thread until get result
  delete poll;
  return 0;
}