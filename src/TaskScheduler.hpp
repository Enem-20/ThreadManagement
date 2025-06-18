#ifndef C_TASK_SCHEDULER_H
#define C_TASK_SCHEDULER_H

#include <vector>
#include <memory>
#include <queue>
#include <stddef.h>
#include <mutex>
#include <thread>
#include <condition_variable>
#include <atomic>

#include "ThreadManagementEXPORTS.hpp"

class Task;

class THREAD_MANAGEMENT_EXPORT TaskScheduler {
public:
    static TaskScheduler* getInstance(size_t threadsCount = 0);
    ~TaskScheduler();

    void pushTask(std::shared_ptr<Task> task);
    void setThreadsCount(size_t newThreadsCount);
    void stop();
private:
    void workerLoop();

    TaskScheduler(size_t threadsCount = 0);
    static TaskScheduler* _instance;

    std::vector<std::thread> _workers;
    std::queue<std::shared_ptr<Task>> _tasks;
    std::mutex _queueMutex;
    std::condition_variable _condition;
    std::atomic<bool> _running{true};
    size_t _currentThreadsCount = 0;
};

#endif