#include "Thread.h"

ThreadPool::ThreadPool(size_t numThreads)
	: m_StopPool(false)
{

	for (size_t i = 0; i < numThreads; ++i) {
		m_Workers.emplace_back(&ThreadPool::workerFunction, this);
	}

}

ThreadPool::~ThreadPool()
{
	stop();
}

void ThreadPool::enqueueTask(Task task)
{
	{
		std::unique_lock<std::mutex> lock(m_QueueMutex);
		m_Tasks.push(task);
	}
	m_Condition.notify_one();
}

void ThreadPool::stop()
{
	{
		std::unique_lock<std::mutex> lock(m_QueueMutex);
		m_StopPool = true;
	}

	m_Condition.notify_all(); // Notify all workers to stop


	for (std::thread& worker : m_Workers) {
		worker.join();

	}
}


void ThreadPool::workerFunction()
{
	while (true) {
		Task t;

		{
			std::unique_lock<std::mutex> lock(m_QueueMutex);

			// wait for a task or the stop signal
			m_Condition.wait(lock, [this] {
				return this->m_StopPool || !this->m_Tasks.empty();
				});

			if (this->m_StopPool && this->m_Tasks.empty())
				return;

			t = std::move(this->m_Tasks.front());
			this->m_Tasks.pop();
		}

		// execute the task outside the lock to minimize lock duration
		t.task();
	}
}

