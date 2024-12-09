/*
 * @Author: Xudong0722
 * @Date: 2024-09-19 23:43:43
 * @Last Modified by: Xudong0722
 * @Last Modified time: 2024-11-10 17:25:02
 */

#include "ThreadPool.h"
#include <iostream>

ThreadPool::ThreadPool(int size)
    : stop_(false)
{
    for (int i = 0; i < size; ++i)
    {
        threads_.emplace_back(std::thread([this]()
                                          {
            while(true)
            {
                std::function<void()> task;
                {
                    std::unique_lock<std::mutex> lock(tasks_mutex_);
                    cv_.wait(lock, [this](){
                        return stop_ || !tasks_.empty();
                    });
                    if(stop_ && tasks_.empty()) return ;
                    task = std::move(tasks_.front());
                    tasks_.pop();
                }
                //std::cout<< __PRETTY_FUNCTION__ << std::endl;
                task();
            } }));
    }
}

ThreadPool::~ThreadPool()
{
    {
        std::unique_lock<std::mutex> lock(tasks_mutex_);
        stop_ = true;
    }
    cv_.notify_all();
    for (std::thread &th : threads_)
    {
        if (th.joinable())
        {
            th.join();
        }
    }
}

void ThreadPool::add(std::function<void()> task)
{
    // std::cout << __PRETTY_FUNCTION__ << std::endl;
    {
        std::unique_lock<std::mutex> lock(tasks_mutex_);
        if (stop_)
        {
            throw std::runtime_error("Thread pool is already stop, can't add task anymore.");
        }
        tasks_.emplace(task);
    }
    cv_.notify_one();
}
