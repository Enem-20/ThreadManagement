#ifndef C_THREAD_HPP
#define C_THREAD_HPP

#include <functional>
#include <string_view>

class ThreadImpl;

class Thread {
	ThreadImpl* _impl;
public:
	Thread(std::string_view name);
public:
	void push(const std::function<void()>& func);
	void execute();

	std::string_view getName() const;
};

#endif // C_THREAD_HPP