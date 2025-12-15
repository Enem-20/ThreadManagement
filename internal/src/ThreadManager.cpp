#include "ThreadManager.hpp"

#include <tsl/hopscotch_map.h>

#include "Thread.hpp"

class ThreadManagerImpl {
	tsl::hopscotch_map<std::string, Thread*> _threads;
public:
	ThreadManagerImpl() {
		
	}
	~ThreadManagerImpl() {
		for(auto& thread : _threads) {
			delete thread.second;
		}

		_threads.clear();
	}

	void emplaceThread(Thread* thread) {
		_threads.emplace(std::string(thread->getName()), thread);
	}

	void update() {
		for(auto& thread : _threads) {
			thread.second->execute();
		}
	}
};

ThreadManager::ThreadManager() {
	_impl = new ThreadManagerImpl();
}

ThreadManager::~ThreadManager() {
	delete _impl;
}

void ThreadManager::emplaceThread(Thread* thread) {
	_impl->emplaceThread(thread);
}

void ThreadManager::update() {
	_impl->update();
}

ThreadManager* ThreadManager::getInstance() {
	if(_instance == nullptr) [[unlikely]] {
		_instance = new ThreadManager();
	}
	return _instance;
}