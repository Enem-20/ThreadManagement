#include "ThreadScheduler.hpp"

#include <thread>
#include <chrono>
#include <iostream>

#include <tsl/hopscotch_map.h>

#include "HotThread.hpp"
#include "EventLoop.hpp"

class ThreadSchedulerImpl {
	tsl::hopscotch_map<std::string, HotThread> _threads;
	EventLoop _loop;
	size_t _threadCount;
	size_t _currentThread = 0;
	bool _exitRequested;
public:
	ThreadSchedulerImpl() {
		_threadCount = std::jthread::hardware_concurrency();
		_threads.reserve(_threadCount);
		for(size_t i = 0; i < _threadCount; ++i) {
			_threads.emplace(std::string("thread") + std::to_string(i), std::move(HotThread())).first.value()
				.execute(100, [](){return false;});
		}
	}

	void execute(size_t intervalMs, const std::function<bool()>& exitCondition) {
		_loop.push([this, exitCondition, intervalMs](){
			_exitRequested &= (!_exitRequested) ? exitCondition() : _exitRequested;
			for(size_t i = 0; (i < _threadCount) && !_exitRequested; ++i) {
				i %= _threadCount;
				_threads[std::string("thread") + std::to_string(i)].push(_loop.extract());
				std::this_thread::sleep_for(std::chrono::milliseconds(intervalMs));
			}
			std::cout << "Thread scheduler exiting...\n";
		});
	}

	void push(const std::function<void()>& task) {
		_loop.push(task);
	}
};

ThreadScheduler::ThreadScheduler() 
	: _impl(new ThreadSchedulerImpl())
{}

ThreadScheduler::~ThreadScheduler() {
	delete _impl;
}

void ThreadScheduler::push(const std::function<void()>& task) {
	_impl->push(task);
}

void ThreadScheduler::execute(size_t intervalMs, const std::function<bool()>& exitCondition) {
	_impl->execute(intervalMs, exitCondition);
}
