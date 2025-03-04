#include "../headers/ThreadPool.h"

ThreadPool::ThreadPool(const size_t numThreads) {
    stop = false;
    this->numThreads = numThreads;
    for (size_t i = 0; i < this->numThreads; i++) {
        workers.emplace_back(&workerThread, this);
    }
}

ThreadPool::~ThreadPool() {
    stop = true;
    cv.notify_all();
    for (std::thread &worker : workers){
        worker.join();
    }
}

void ThreadPool::queueTask(const std::function<void()>& task) {
    {
        std::lock_guard lock (queueMutex);
        tasks.push(task);
    }
    cv.notify_one();
}

void ThreadPool::waitForAllTasks() {
    std::unique_lock lock(queueMutex);
    finished_condition.wait(lock, [this]() {return tasks.empty();});
}

size_t ThreadPool::getThreadCount() const {
    return this->numThreads;
}

void ThreadPool::workerThread() {
    while (true) {
        std::function<void()> task;
        {
            std::unique_lock lock(queueMutex);
            cv.wait(lock, [this]() {return (!tasks.empty()) || stop;});

            if (stop && tasks.empty()) {
                finished_condition.notify_all();
                return;
            }

            task = std::move(tasks.front());
            tasks.pop();
        }

        task();
        {
            std::lock_guard lock(queueMutex);
            if (tasks.empty()) {
                finished_condition.notify_all();
            }
        }
    }
}



