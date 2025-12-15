#include "Thread.hpp"

#include <iostream>
#include <thread>

#include "EventLoop.hpp"
#include "ThreadManager.hpp"

class ThreadImpl {
	std::string _name;
	EventLoop _loop;
	std::jthread* _thread;
	size_t _id;
public:
	ThreadImpl(std::string_view name) 
		: _name(name)
		, _thread(nullptr)
		, _id(0)
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
		_id = std::hash<std::jthread::id>{}(std::this_thread::get_id());
	}

	std::string_view getName() const {
		return _name;
	}

	size_t getId() const {
		return _id;
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

size_t Thread::getId() const {
	return _impl->getId();
}