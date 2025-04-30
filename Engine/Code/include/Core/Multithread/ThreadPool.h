#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <vector>
#include <queue>
#include <thread>
#include <functional>
#include <mutex>
#include <condition_variable>
#include <atomic>

#include "EngineExport.h"

namespace Engine
{
	namespace Core
	{
		namespace Multithread
		{
			class ThreadPool
			{
			public:
				ENGINE_API ThreadPool() : ThreadPool(10) {}
				ENGINE_API explicit ThreadPool(uint32_t a_nbThreads);
				ENGINE_API ~ThreadPool();

				ENGINE_API void Enqueue(std::function<void()> const& a_func);
				ENGINE_API void WaitUntilFinished();

			private:
				void CheckQueue();

				std::vector<std::thread> m_workers;
				std::deque<std::function<void()>> m_taskQueue;
				std::mutex m_queueMutex;
				std::condition_variable m_waitCondition;
				std::condition_variable m_finishCondition;
				std::atomic<bool> m_stop{ false };
				std::atomic<int> m_tasksRemaining{ 0 };
				uint32_t m_nbThreads = 0;
			};
		}
	}
}
#endif // THREADPOOL_H
