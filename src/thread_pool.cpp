#include "cpptp/cpptp.hpp"

#include <thread>

namespace cpptp
{
    ThreadPool::ThreadPool()
        : m_Workers(std::thread::hardware_concurrency()), m_Count(0)
    {
    }

    ThreadPool::ThreadPool(size_type threadCount)
        : m_Workers(threadCount), m_Count(0)
    {
    }

    ThreadPool::size_type ThreadPool::workers() const noexcept
    {
        return m_Workers.size();
    }

    ThreadPool::size_type ThreadPool::pending_tasks() const noexcept
    {
        size_type pending = 0;

        for (const auto& worker : m_Workers)
        {
            pending += worker.pending_tasks();
        }

        return pending;
    }

    void ThreadPool::stop()
    {
        for (auto& worker : m_Workers)
        {
            worker.stop();
        }
    }

    void ThreadPool::await()
    {
        for (auto& worker : m_Workers)
        {
            worker.await();
        }
    }
} // namespace cpptp