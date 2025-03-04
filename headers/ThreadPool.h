//
// Created by MY PC on 3/4/2025.
//

#ifndef THREADPOOL_H
#define THREADPOOL_H
#include <condition_variable>
#include <functional>
#include <queue>
#include <thread>
#include <vector>

class ThreadPool {
public:
    explicit ThreadPool(const size_t numThreads);
    ~ThreadPool();
    void queueTask(const std::function<void()>& task);
    void waitForAllTasks();
    size_t getThreadCount() const;

private:
    std::vector<std::thread> workers;
    std::queue<std::function<void()>> tasks;
    size_t numThreads;
    std::mutex queueMutex;
    std::condition_variable cv;
    std::condition_variable finished_condition;
    std::atomic<bool> stop;

    void workerThread();
};

#endif //THREADPOOL_H
