#ifndef C_THREAD_SCHEDULER_HPP
#define C_THREAD_SCHEDULER_HPP

#include <functional>

class ThreadSchedulerImpl;

class ThreadScheduler {
	ThreadSchedulerImpl* _impl;
public:
	ThreadScheduler();
	~ThreadScheduler();

	void push(const std::function<void()>& task);
	void execute(size_t intervalMs = 100, const std::function<bool()>& exitCondition = [](){return false;});
};

#endif // C_THREAD_SCHEDULER_HPP