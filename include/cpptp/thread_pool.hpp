#pragma once

#include <atomic>
#include <future>
#include <memory>
#include <vector>
#include <type_traits>

#include "cpptp/worker.hpp"

namespace cpptp
{
    class ThreadPool
    {
    public:
        using size_type = std::size_t;

        /**
         * Creates thread pool with the available processor count
         */
        ThreadPool();

        /**
         * Creates thread pool with provided number of worker threads
         * @param thread_count Number of how many threads to use in pool
         */
        explicit ThreadPool(size_type thread_count);

        ThreadPool(const ThreadPool&) = delete;
        ThreadPool& operator=(const ThreadPool&) = delete;

    public:
        /**
         * Method used to view how many workers are used for work.
         * Worker count will be the same as provided thread count in class constructor.
         * @return Number of worker threads in this pool
         */
        [[nodiscard]] size_type workers() const noexcept;

        /**
         * Retrieves approximate number of tasks that workers have remaining
         * @return Approximate result of pending tasks
         */
        [[nodiscard]] size_type pending_tasks() const;

        /**
         * Forces all workers to deny new tasks and finish with pending ones
         */
        void stop() noexcept;

        /**
         * Blocks until there are no more tasks waiting to be executed for all workers.
         * Use this method only if other threads cannot add more tasks
         */
        void await();

        /**
         * Explicitly acquire worker from poll.
         * Useful if certain tasks are required to execute on the same thread
         * @return Existing worker instance from pool
         */
        [[nodiscard]] std::shared_ptr<Worker> acquire_worker();

        /**
         * Takes provided callable with arguments and executes it on worker thread.
         * All arguments will be copied to and forwarded to provided callable.
         * If returned value is not necessary, more effective strategy will be calling execute method with the same arguments
         * @param fn Callable instance that needs to be executed
         * @param args Arguments passed to given callable
         * @return future of executing provided callable with arguments
         * @throws std::runtime_error if raising exceptions are enabled and pool has been stopped
         * @see execute
         */
        template<class F, class... Args>
        std::future<std::invoke_result_t<F, Args...>> submit(F&& fn, Args&&... args)
        {
            auto index = m_Count.fetch_add(1, std::memory_order_relaxed) % workers();

            return m_Workers[index]->submit(std::forward<F>(fn), std::forward<Args>(args)...);
        }

        /**
         * Takes given callable with arguments and executes it on worker thread.
         * All arguments will be copied and forwarded to given callable.
         * Since this method does not create future it is more preferable to use than submit if result is unnecessary
         * @param fn Callable instance that needs to be executed
         * @param args Arguments passed to given callable
         * @throws std::runtime_error if raising exceptions are enabled and pool has been stopped
         * @see submit
         */
        template<class F, class... Args>
        void execute(F&& fn, Args&&... args)
        {
            auto index = m_Count.fetch_add(1, std::memory_order_relaxed) % workers();

            m_Workers[index]->execute(std::forward<F>(fn), std::forward<Args>(args)...);
        }

        /**
         * Applies the given function in separate threads to the result of dereferencing every iterator in range [first, last).
         * @param first First iterator in range
         * @param last Last iterator in range
         * @param fn Function object for which to apply dereferenced values from range [first, last)
         */
        template<class InputIt, class UnaryFunction>
        void for_each(InputIt first, InputIt last, UnaryFunction fn)
        {
            for (; first != last; ++first)
            {
                execute(fn, std::ref(*first));
            }
        }

    private:
        std::vector<std::shared_ptr<Worker>> m_Workers;
        std::atomic<size_type> m_Count;
    };
} // namespace cpptp
