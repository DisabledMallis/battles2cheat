#include "shared_thread.h"

#include <Windows.h>

shared_thread::shared_thread(int timeoutMillis)
{
	this->innerThread = std::thread([this]() {
		while (true) {
			if (workQueue.size() > 0) {
				std::function<void()> work = workQueue.front();
				work();
				workQueue.pop_front();
			}
			else {
				Sleep(this->timeoutMillis);
			}
		}
	});
	this->innerThread.detach();
}

void shared_thread::DoWork(std::function<void()> work)
{
	this->workQueue.push_back(work);
}