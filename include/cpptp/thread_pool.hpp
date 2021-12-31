#pragma once

#include <atomic>
#include <future>
#include <vector>
#include <type_traits>

#include "cpptp/worker.hpp"

namespace cpptp
{
    /*
     * Single producer thread pool
     */
    class ThreadPool
    {
    public:
        using size_type = std::size_t;

        ThreadPool();
        explicit ThreadPool(size_type threadCount);

        ThreadPool(const ThreadPool&) = delete;
        ThreadPool& operator= (const ThreadPool&) = delete;

    public:
        size_type workers() const noexcept;
        size_type pending_tasks() const noexcept;
        void stop();
        void await();

        template<class F, class ... Args>
        std::future<std::result_of_t<F(Args...)>> submit(F&& f, Args&& ... args)
        {
            auto index = m_Count.fetch_add(1, std::memory_order_relaxed) % workers();

            return m_Workers[index].submit(std::forward<F>(f), std::forward<Args>(args)...);
        }

        template<class F, class ... Args>
        void execute(F&& f, Args&& ... args)
        {
            auto index = m_Count.fetch_add(1, std::memory_order_relaxed) % workers();

            m_Workers[index].execute(std::forward<F>(f), std::forward<Args>(args)...);
        }

        template<class InputIt, class UnaryFunction>
        void for_each(InputIt first, InputIt last, UnaryFunction f)
        {
            for (; first != last; ++first)
            {
                execute(f, std::ref(*first));
            }
        }

    private:
        std::vector<Worker> m_Workers;
        std::atomic<size_type> m_Count;
    };
} // namespace cpptp