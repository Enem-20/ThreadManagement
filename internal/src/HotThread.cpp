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
public:
	~HotThreadImpl() {
		if(_thread) [[unlikely]]
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
	}

	void requestExit() {
		_exitRequested = true;
	}

	bool exiting() {
		return _exitRequested;
	}

	void push(const std::function<void()>& task) {
		_loop.push(task);
	}
};

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