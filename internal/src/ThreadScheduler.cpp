#include "ThreadScheduler.hpp"

#include <thread>
#include <chrono>
#include <iostream>

#include <tsl/hopscotch_map.h>

#include "HotThread.hpp"
#include "EventLoop.hpp"

class ThreadSchedulerImpl {
	tsl::hopscotch_map<std::string, HotThread> _threads;
	tsl::hopscotch_map<size_t, HotThread*> _threadsById;
	EventLoop _loop;
	size_t _threadCount;
	size_t _currentThread = 0;
	bool _exitRequested;
public:
	ThreadSchedulerImpl() {
		_threadCount = std::jthread::hardware_concurrency() - 1;
		_threads.reserve(_threadCount);
		for(size_t i = 0; i < _threadCount; ++i) {
			auto& hotThread = _threads.emplace(std::string("thread") + std::to_string(i), std::move(HotThread())).first.value();
			_threadsById.emplace(hotThread.getId(), &hotThread);
		}
	}

	// Should be called in main thread only
	void initialize() {
		std::thread::id mainThreadId = std::this_thread::get_id();
		auto& mainThread = _threads.emplace("mainThread", HotThread(std::hash<std::thread::id>{}(mainThreadId))).first.value();
		_threadsById.emplace(mainThread.getId(), &mainThread);
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

	HotThread* getThreadById(size_t id) {
		auto it = _threadsById.find(id);
		if(it != _threadsById.end()) {
			return it->second;
		}
		return nullptr;
	}
};

ThreadScheduler::ThreadScheduler() 
	: _impl(new ThreadSchedulerImpl())
{}

ThreadScheduler::~ThreadScheduler() {
	delete _impl;
}

void ThreadScheduler::initialize() {
	_impl->initialize();
}

void ThreadScheduler::push(const std::function<void()>& task) {
	_impl->push(task);
}

void ThreadScheduler::execute(size_t intervalMs, const std::function<bool()>& exitCondition) {
	_impl->execute(intervalMs, exitCondition);
}

HotThread* ThreadScheduler::getThreadById(size_t id) {
	return _impl->getThreadById(id);
}