#include "MainThread.hpp"

#include <iostream>
#include <thread>

#include "EventLoop.hpp"

MainThread::MainThread(size_t id) 
	: HotThread(id)
{}

MainThread::~MainThread() {

}

void MainThread::execute(size_t sleepMs, const std::function<bool()>& stopCondition) {
	EventLoop loop;
	while(!stopCondition() && !requestedExit()) {
		loop.execute();
		std::this_thread::sleep_for(std::chrono::milliseconds(sleepMs));
	}
	requestExit();
	std::cout << "Exitting from hot thread\n";
}