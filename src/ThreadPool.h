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
#include <future>

#define DEFAULT_THREAD_POOL_SIZE 10
class ThreadPool
{
public:
    ThreadPool(int size = DEFAULT_THREAD_POOL_SIZE);
    ~ThreadPool();

    template <class F, class... Args>
    auto add(F &&f, Args &&...args)
        -> std::future<typename std::result_of<F(Args...)>::type>;

private:
    std::vector<std::thread> threads_;
    std::queue<std::function<void()>> tasks_;
    std::mutex tasks_mutex_;
    std::condition_variable cv_;
    bool stop_;
};

template <class F, class... Args>
auto ThreadPool::add(F &&f, Args &&...args) -> std::future<typename std::result_of<F(Args...)>::type>
{
    using return_type = typename std::result_of<F(Args...)>::type;

    auto task = std::make_shared<std::packaged_task<return_type()>>(
        std::bind(std::forward<F>(f), std::forward<Args>(args)...));

    std::future<return_type> res = task->get_future();
    {
        std::unique_lock<std::mutex> lock(tasks_mutex_);
        if (stop_)
            throw std::runtime_error("enqueue on stopped ThreadPool!");
        tasks_.emplace([task]
                       { (*task)(); });
    }
    cv_.notify_one();
    return res;
}