#ifndef THREAD_H
#define THREAD_H


#include <iostream>
#include <vector>
#include <thread>
#include <queue>
#include <functional>
#include <atomic>
#include <condition_variable>
#include "Block.h"


struct Task {
	std::function<void()> task;
};

class ThreadPool {
public:
	ThreadPool(size_t numThreads);
	~ThreadPool();
	void enqueueTask(Task);
	void stop();

private:

	void workerFunction();

	std::vector<std::thread> m_Workers;
	std::queue<Task> m_Tasks;
	std::mutex m_QueueMutex;
	std::condition_variable m_Condition;
	std::atomic<bool> m_StopPool;

};





#endif // 
