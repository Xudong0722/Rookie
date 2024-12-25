/*
 * @Author: Xudong0722
 * @Date: 2024-09-01 13:34:35
 * @Last Modified by: Xudong0722
 * @Last Modified time: 2024-09-01 13:48:58
 */
#pragma once

void errif(bool condition, const char *message);

class noncopyable
{
protected:
  noncopyable() {}
  ~noncopyable() {}
private:
  noncopyable(const noncopyable&) = delete;
  noncopyable& operator=(const noncopyable&) = delete;
};

class noncopymoveable
{
protected:
  noncopymoveable() {}
  ~noncopymoveable() {}
private:
  noncopymoveable(const noncopymoveable&) = delete;
  noncopymoveable(noncopymoveable&&) = delete;
  noncopymoveable& operator=(const noncopymoveable&) = delete;
  noncopymoveable& operator=(noncopymoveable&&) = delete;
};

