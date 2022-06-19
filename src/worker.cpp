#include "cpptp/worker.hpp"

namespace cpptp
{
    static void end_task()
    {
    }

    Worker::Worker()
        : m_Stopped(false), m_RemainingTasks(0)
    {
        m_WorkerThread = std::thread([this] {
            while (!stopped() || pending_task_count() > 0)
            {
                task_type task;

                m_Tasks.wait_dequeue(task);
                task();

                m_RemainingTasks.fetch_sub(1, std::memory_order_acq_rel);
                m_WaitingCv.notify_all();
            }
        });
    }

    Worker::~Worker()
    {
        stop();

        if (m_Tasks.enqueue(end_task))
        {
            m_RemainingTasks.fetch_add(1, std::memory_order_relaxed);
        }
       
        m_WorkerThread.join();
    }

    void Worker::stop() noexcept
    {
        m_Stopped.store(true, std::memory_order_release);
    }

    bool Worker::stopped() const noexcept
    {
        return m_Stopped.load(std::memory_order_acquire);
    }

    Worker::size_type Worker::pending_task_count() const
    {
        return m_Tasks.size_approx();
    }

    void Worker::wait()
    {
        std::unique_lock l(m_WaitingMutex);
        m_WaitingCv.wait(l, [this] {
            return m_RemainingTasks.load(std::memory_order_acquire) <= 0;
        });
    }
} // namespace cpptp