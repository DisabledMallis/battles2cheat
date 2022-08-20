#pragma once

#include <functional>
#include <thread>
#include <deque>

class shared_thread {
	std::thread innerThread;
	int timeoutMillis;
	std::deque<std::function<void()>> workQueue;
public:
	shared_thread(int timeoutMillis);
	void DoWork(std::function<void()> work);
};