#ifndef C_EVENT_LOOP_HPP
#define C_EVENT_LOOP_HPP

#include <functional>

class EventLoopImpl;

class EventLoop {
	EventLoopImpl* _impl;
public:
	EventLoop();
	~EventLoop();

	void push(const std::function<void()>& func);
	void pushEvent(const std::function<void()>& func);
	void execute();
	std::function<void()> extract();

	EventLoopImpl* operator->() {
		return _impl;
	}
};

#endif // C_EVENT_LOOP_HPP