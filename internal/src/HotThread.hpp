#ifndef C_HOT_THREAD_HPP
#define C_HOT_THREAD_HPP

#include <cstdint>
#include <functional>

class HotThreadImpl;

class HotThread {
	HotThreadImpl* _impl;
public:
	HotThread();
	HotThread(HotThread&& other) noexcept;

	~HotThread();

	void execute(size_t sleepMs = 100, const std::function<bool()>& stopCondition = [](){return false;});
	void requestExit();

	void push(const std::function<void()>& task);
	
	size_t getId() const;
};

#endif // C_HOT_THREAD_HPP