#include "cpptp/cpptp.hpp"

namespace cpptp
{
    static std::vector<std::shared_ptr<Worker>> create_workers(std::size_t count)
    {
        count = (count == 0 ? 1 : count);

        std::vector<std::shared_ptr<Worker>> workers;
        workers.reserve(count);

        for (std::size_t i = 0; i < count; i++)
        {
            workers.push_back(std::make_shared<Worker>());
        }

        return workers;
    }

    ThreadPool::ThreadPool()
        : m_Workers(std::move(create_workers(std::thread::hardware_concurrency()))), m_Count(0)
    {
    }

    ThreadPool::ThreadPool(size_type thread_count)
        : m_Workers(std::move(create_workers(thread_count))), m_Count(0)
    {
    }

    ThreadPool::size_type ThreadPool::worker_count() const noexcept
    {
        return m_Workers.size();
    }

    ThreadPool::size_type ThreadPool::pending_task_count() const
    {
        size_type pending = 0;

        for (const auto& worker : m_Workers)
        {
            pending += worker->pending_task_count();
        }

        return pending;
    }

    void ThreadPool::stop() noexcept
    {
        for (auto& worker : m_Workers)
        {
            worker->stop();
        }
    }

    std::shared_ptr<Worker> ThreadPool::acquire_worker()
    {
        auto index = m_Count.fetch_add(1, std::memory_order_relaxed) % worker_count();

        return m_Workers[index];
    }
} // namespace cpptp