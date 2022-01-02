#include "cpptp/worker.hpp"

namespace cpptp
{
    Worker::Worker()
        : m_Stopped(false)
    {
        m_WorkerThread = std::thread([this] {
            task_type task;

            while (true)
            {
                {
                    std::unique_lock<std::mutex> l(m_Mutex);
                    m_ConditionVariable.wait(l, [this] {
                        return !m_Tasks.empty() || stopped();
                    });

                    if (m_Tasks.empty() && stopped())
                    {
                        break;
                    }

                    task = std::move(m_Tasks.front());
                    m_Tasks.pop();
                }

                task();
                m_ConditionVariable.notify_one();
            }
        });
    }

    Worker::~Worker()
    {
        stop();
        m_WorkerThread.join();
    }

    void Worker::stop() noexcept
    {
        m_Stopped.store(true, std::memory_order_release);
        m_ConditionVariable.notify_one();
    }

    bool Worker::stopped() const noexcept
    {
        return m_Stopped.load(std::memory_order_acquire);
    }

    Worker::size_type Worker::pending_tasks() const
    {
        std::lock_guard<std::mutex> l(m_Mutex);
        return m_Tasks.size();
    }

    void Worker::await()
    {
        std::unique_lock<std::mutex> l(m_Mutex);
        m_ConditionVariable.wait(l, [this] {
            return m_Tasks.empty();
        });
    }
} // namespace cpptp