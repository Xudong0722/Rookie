/*
 * @Author: Xudong0722
 * @Date: 2024-09-19 23:43:40
 * @Last Modified by: Xudong0722
 * @Last Modified time: 2024-10-28 18:25:19
 */
#pragma once

#include <thread>
#include <functional>
#include <vector>
#include <queue>
#include <condition_variable>

#define DEFAULT_THREAD_POOL_SIZE 10
class ThreadPool
{
public:
    ThreadPool(int size = DEFAULT_THREAD_POOL_SIZE);
    ~ThreadPool();

    void add(std::function<void()>);

private:
    std::vector<std::thread> threads_;
    std::queue<std::function<void()>> tasks_;
    std::mutex tasks_mutex_;
    std::condition_variable cv_;
    bool stop_;
};