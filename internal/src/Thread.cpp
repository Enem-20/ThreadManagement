#include "Thread.hpp"

#include <iostream>
#include <thread>

#include "EventLoop.hpp"
#include "ThreadManager.hpp"

class ThreadImpl {
	EventLoop _loop;
	std::jthread* _thread;
	std::string _name;
public:
	ThreadImpl(std::string_view name) 
		: _name(name)
	{
		std::cout << "Thread impl created\n";
	}
	~ThreadImpl() {
		delete _thread;
	}
public:
	void push(const std::function<void()>& func) {
		_loop.push(func);
	}

	void execute() {
		_thread = new std::jthread(std::bind(&EventLoop::execute, &_loop));
	}

	std::string_view getName() const {
		return _name;
	}
};

Thread::Thread(std::string_view name) 
	: _impl(new ThreadImpl(name))
{
	ThreadManager::getInstance()->emplaceThread(this);
}

void Thread::push(const std::function<void()>& func) {
	_impl->push(func);
}
	
void Thread::execute() {
	_impl->execute();
}

std::string_view Thread::getName() const {
	return _impl->getName();
}