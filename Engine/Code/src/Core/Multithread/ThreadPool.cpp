#include "Core/Multithread/ThreadPool.h"

namespace Engine
{
	namespace Core
	{
		namespace Multithread
		{
            ThreadPool::ThreadPool(const uint32_t a_nbThreads) : m_nbThreads(a_nbThreads)
            {
                for (uint32_t i = 0; i < m_nbThreads; i++)
                    m_workers.emplace_back(&ThreadPool::CheckQueue, this);
            }

            ThreadPool::~ThreadPool()
            {
                m_stop.store(true, std::memory_order_release);
                m_waitCondition.notify_all();

                for (auto& t_worker : m_workers)
                    if (t_worker.joinable())
                        t_worker.join();

                m_workers.clear();
            }

            void ThreadPool::Enqueue(std::function<void()> const& a_func)
            {
                m_tasksRemaining.fetch_add(1, std::memory_order_acq_rel);
                {
                    std::unique_lock t_lock(m_queueMutex);
                    m_taskQueue.emplace_back(a_func);
                }
                m_waitCondition.notify_one();
            }

            void ThreadPool::WaitUntilFinished()
            {
                std::unique_lock t_lock(m_queueMutex);
                m_finishCondition.wait(t_lock, [this]() { return m_tasksRemaining.load(std::memory_order_acquire) == 0 && m_taskQueue.empty(); });
            }

            void ThreadPool::CheckQueue()
            {
                while (true)
                {
                    std::function<void()> t_task;

                    {
                        std::unique_lock t_lock(m_queueMutex);
                        m_waitCondition.wait(t_lock, [this]() { return m_stop.load() || !m_taskQueue.empty(); });

                        if (m_stop.load() && m_taskQueue.empty())
                            return;

                        t_task = std::move(m_taskQueue.front());
                        m_taskQueue.pop_front();
                    }

                    t_task();
                    m_tasksRemaining.fetch_sub(1, std::memory_order_acq_rel);
                    m_finishCondition.notify_one();
                }
            }
		}
	}
}