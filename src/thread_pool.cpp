#include "cpptp/cpptr.hpp"

#include <thread>

namespace cpptp
{
    ThreadPool::ThreadPool()
            : m_Workers(std::thread::hardware_concurrency()), m_Count(0)
    {
    }

    ThreadPool::ThreadPool(std::size_t threadCount)
            : m_Workers(threadCount), m_Count(0)
    {
    }

    void ThreadPool::stop()
    {
        for (auto& worker : m_Workers)
        {
            worker.stop();
        }
    }
} // namespace cpptp