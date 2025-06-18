#include "TaskScheduler.hpp"

#include <iostream>

#include "Task.hpp"

TaskScheduler* TaskScheduler::_instance = nullptr;

TaskScheduler* TaskScheduler::getInstance(size_t threadsCount) {
    if(!_instance)
        return _instance = new TaskScheduler(threadsCount);
    return _instance;
}

TaskScheduler::~TaskScheduler() {
    stop();
}

void TaskScheduler::stop() {
    _running = false;
    _condition.notify_all();

    for (auto& t : _workers) {
        if (t.joinable())
            t.join();
    }
}

void TaskScheduler::pushTask(std::shared_ptr<Task> task) {
    {
        std::lock_guard<std::mutex> lock(_queueMutex);
        _tasks.push(std::move(task));
    }
    _condition.notify_one();
}

void TaskScheduler::setThreadsCount(size_t newCount) {
    stop();
    _workers.clear();
    _running = true;
    for (size_t i = 0; i < newCount; ++i) {
        _workers.emplace_back(&TaskScheduler::workerLoop, this);
    }
}

void TaskScheduler::workerLoop() {
    while (_running) {
        std::shared_ptr<Task> task;
        {
            std::unique_lock<std::mutex> lock(_queueMutex);
            _condition.wait(lock, [&] { return !_tasks.empty() || !_running; });

            if (!_running && _tasks.empty())
                return;

            task = std::move(_tasks.front());
            _tasks.pop();
        }

        if (task && task->valid()) {
            std::cout << "run task\n";
            (*task)();
        }
    }
}


TaskScheduler::TaskScheduler(size_t threadsCount)
{
    if (!threadsCount)
        threadsCount = std::thread::hardware_concurrency();

    _currentThreadsCount = threadsCount;

    for (size_t i = 0; i < threadsCount; ++i) {
        _workers.emplace_back(&TaskScheduler::workerLoop, this);
    }
}