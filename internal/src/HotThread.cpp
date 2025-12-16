#include "HotThread.hpp"

#include <thread>
#include <chrono>
#include <iostream>

#include "EventLoop.hpp"

using namespace std::chrono_literals;

class HotThreadImpl {
	EventLoop _loop;
	std::jthread* _thread;
	bool _exitRequested;
	size_t _id;
public:
	HotThreadImpl(size_t id) 
		: _thread(nullptr)
		, _exitRequested(false)
		, _id(id)
	{
		
	}

	HotThreadImpl() 
		: _thread(nullptr)
		, _exitRequested(false)
		, _id(0)
	{}

	~HotThreadImpl() {
		if(_thread) [[likely]]
			delete _thread;
	}

	void execute(size_t sleepMs = 100, const std::function<bool()>& stopCondition = [](){return false;}) {
		_thread = new std::jthread([this, sleepMs, stopCondition](){
			_exitRequested &= (!_exitRequested) ? stopCondition() : _exitRequested;
			while(!_exitRequested) {
				_loop.execute();
				std::this_thread::sleep_for(std::chrono::milliseconds(sleepMs));
			}
			_exitRequested = true;
			std::cout << "Exitting from hot thread\n";
		});
		_id = std::hash<std::jthread::id>{}(_thread->get_id());
	}

	void requestExit() {
		_exitRequested = true;
	}

	bool requestedExit() const {
		return _exitRequested;
	}

	void push(const std::function<void()>& task) {
		_loop.push(task);
	}

	size_t getId() const {
		return _id;
	}
};

HotThread::HotThread(size_t id) 
	: _impl(new HotThreadImpl(id))
{}

HotThread::HotThread() 
	: _impl(new HotThreadImpl())
{}

HotThread::HotThread(HotThread&& other) noexcept
	: _impl(other._impl)
{}

HotThread::~HotThread() {
	delete _impl;
}

void HotThread::execute(size_t sleepMs, const std::function<bool()>& stopCondition) {
	_impl->execute(sleepMs, stopCondition);
}

void HotThread::requestExit() {
	_impl->requestExit();
}

void HotThread::push(const std::function<void()>& task) {
	_impl->push(task);
}

size_t HotThread::getId() const {
	return _impl->getId();
}

bool HotThread::requestedExit() const {
	return _impl->requestedExit();
}