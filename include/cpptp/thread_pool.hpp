#pragma once

#include <atomic>
#include <future>
#include <vector>
#include <type_traits>

#include "cpptp/worker.hpp"

namespace cpptp
{
    class ThreadPool
    {
    public:
        ThreadPool();
        explicit ThreadPool(std::size_t threadCount);

        ThreadPool(const ThreadPool&) = delete;
        ThreadPool& operator= (const ThreadPool&) = delete;

    public:
        template<class F, class ... Args>
        std::future<std::result_of_t<F(Args...)>> submit(F&& f, Args&& ... args)
        {
            auto index = m_Count.fetch_add(1, std::memory_order_relaxed) % m_Workers.size();

            return m_Workers[index].submit(std::forward<F>(f), std::forward<Args>(args)...);
        }

        void stop();
    private:
        std::vector<Worker> m_Workers;
        std::atomic<std::size_t> m_Count;
    };
} // namespace cpptp