#pragma once

#include <mutex>
#include <queue>
#include <atomic>
#include <future>
#include <memory>
#include <thread>
#include <functional>
#include <type_traits>
#include <condition_variable>

#include <blockingconcurrentqueue.h>

namespace cpptp
{
    class Worker
    {
    public:
        using size_type = std::size_t;
        using task_type = std::function<void()>;

        /**
         * Creates new worker instance with work thread and task queue
         */
        Worker();

        Worker(const Worker&) = delete;
        Worker& operator=(const Worker&) = delete;

        /**
         * On destruction worker finishes all unfinished tasks and destroys underlying thread.
         */
        ~Worker();

    public:
        /**
         * Stops worker instance.
         * Any subsequent requests to add more tasks will be rejected
         */
        void stop() noexcept;

        /**
         * Checks if worker has been stopped
         * @return true if worker is no longer active and false otherwise
         */
        [[nodiscard]] bool stopped() const noexcept;

        /**
         * Retrieves approximate number of tasks that worker has remaining
         * @return Approximate number of tasks in queue
         */
        [[nodiscard]] size_type pending_task_count() const;

        /**
        * Wait until all pending tasks are executed
        */
        void wait();

        /**
         * Submits the given callable with arguments to the work queue.
         * All arguments will be copied to and forwarded to provided callable.
         * If returned future does not require waiting, more effective strategy will be calling execute method with the same arguments
         * @param function Callable instance that needs to be executed
         * @param args Arguments passed to given callable
         * @return future of executing provided callable with arguments
         * @throws std::runtime_error if raising exceptions are enabled and worker has been stopped
         * @see execute
         */
        template<class F, class... Args>
        std::future<std::invoke_result_t<F, Args...>> submit(F&& fn, Args&&... args)
        {
            auto task = std::make_shared<std::packaged_task<std::invoke_result_t<F, Args...>(Args...)>>(std::forward<F>(fn));
            auto task_args = std::make_tuple(std::forward<Args>(args)...);

            if (!stopped())
            {
                bool success = m_Tasks.enqueue([=, task_args = std::move(task_args)] () mutable {
                    std::apply(*task, task_args);
                });

                if (success)
                {
                    m_RemainingTasks.fetch_add(1, std::memory_order_relaxed);
                }
            }
#if !defined(CPPTP_DISABLE_EXCEPTIONS)
            else
            {
                throw std::runtime_error("Worker instance has been stopped");
            }
#endif
            return task->get_future();
        }

        /**
         * Submits the given callable with arguments to the work queue.
         * All arguments will be copied and forwarded to given callable.
         * Since this method does not create future it is more preferable to use than submit if result is unnecessary
         * @param fn Callable instance that needs to be executed
         * @param args Arguments passed to given callable
         * @throws std::runtime_error if raising exceptions are enabled and worker has been stopped
         * @see submit
         */
        template<class F, class... Args>
        void execute(F&& fn, Args&&... args)
        {
            if (!stopped())
            {
                auto task = fn;
                auto task_args = std::make_tuple(std::forward<Args>(args)...);

                bool success = m_Tasks.enqueue([task = std::move(task), task_args = std::move(task_args)] () mutable {
                    try
                    {
                        std::apply(task, task_args);
                    } catch (...)
                    {
                    }
                });

                if (success)
                {
                    m_RemainingTasks.fetch_add(1, std::memory_order_relaxed);
                }
            }
#if !defined(CPPTP_DISABLE_EXCEPTIONS)
            else
            {
                throw std::runtime_error("Worker instance has been stopped");
            }
#endif
        }

    private:
        std::mutex m_WaitingMutex;
        std::condition_variable m_WaitingCv;
        moodycamel::BlockingConcurrentQueue<task_type> m_Tasks;
        std::thread m_WorkerThread;
        std::atomic<int64_t> m_RemainingTasks;
        std::atomic<bool> m_Stopped;
    };
} // namespace cpptp