#include "EventLoop.hpp"

#include <list>
#include <functional>

// template<class ... Args>
// std::tuple<Args...> extract(Args... args) {
// 	return std::make_tuple(args...);
// }

// #define DECLARE_IMPL_METHOD(returnType, class, func, args) returnType class::func(args) {	\
// 	_impl->func(std::ap);																	\
// }

#define DEFINE_PIMPL_METHOD(class, MethodName, ...) \
	auto class::MethodName(__VA_ARGS__) -> decltype(_impl->MethodName(__VA_ARGS__)) { \
		return _impl->MethodName(__VA_ARGS__);									\
	}


class EventLoopImpl {
	std::list<std::function<void()>> _queue;
	std::list<std::function<void()>> _repeatable;
public:
	EventLoopImpl()
	{}

	void push(const std::function<void()>& func) {
		_queue.push_back(func);
	}
	void pushEvent(const std::function<void()>& func) {
		_repeatable.push_back(func);
	}
	void execute() {
		while(!_queue.empty()) {
		extract()();
		if(_queue.empty())
			std::copy(_repeatable.begin(), _repeatable.end(), _queue.end());
		}
	}
	std::function<void()> extract() {
		std::function<void()> func = _queue.front();
		_queue.pop_front();
		return func;
	}
};


EventLoop::EventLoop() 
	: _impl(new EventLoopImpl())
{}

EventLoop::~EventLoop() {
	delete _impl;
}

void EventLoop::push(const std::function<void()>& func) {
	_impl->push(func);
}

void EventLoop::pushEvent(const std::function<void()>& func) {
	_impl->pushEvent(func);
}

void EventLoop::execute() {
	_impl->execute();
}

std::function<void()> EventLoop::extract() {
	return _impl->extract();
}