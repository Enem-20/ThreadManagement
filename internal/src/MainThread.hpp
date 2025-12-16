#ifndef C_MAIN_THREAD_HPP
#define C_MAIN_THREAD_HPP

#include "HotThread.hpp"

class MainThread : public HotThread {
public:
	MainThread(size_t id);
	~MainThread();

	void execute(size_t sleepMs = 100, const std::function<bool()>& stopCondition = [](){return false;}) override;

	void requestExit();

	void push(const std::function<void()>& task);
	
	size_t getId() const;
};

#endif // C_MAIN_THREAD_HPP