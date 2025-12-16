#ifndef C_THREAD_SCHEDULER_HPP
#define C_THREAD_SCHEDULER_HPP

#include <functional>

class ThreadSchedulerImpl;
class HotThread;

class ThreadScheduler {
	ThreadSchedulerImpl* _impl;
public:
	ThreadScheduler();
	~ThreadScheduler();

	// Should be called in main thread only
	void initialize();

	void push(const std::function<void()>& task);
	void execute(size_t intervalMs = 100, const std::function<bool()>& exitCondition = [](){return false;});

	HotThread* getThreadById(size_t id);
};

#endif // C_THREAD_SCHEDULER_HPP