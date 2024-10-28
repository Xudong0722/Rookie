#include "ThreadPool.h"
/*
 * @Author: Xudong0722
 * @Date: 2024-09-19 23:43:43
 * @Last Modified by: Xudong0722
 * @Last Modified time: 2024-10-28 18:27:14
 */

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
                    task = tasks_.front();
                    tasks_.pop();
                }
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
