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

namespace cpptp
{
    class Worker
    {
    public:
        using size_type = std::size_t;
        using task_type = std::function<void()>;

        Worker();

        Worker(const Worker&) = delete;
        Worker& operator=(const Worker&) = delete;

        ~Worker();

    public:
        void stop() noexcept;
        bool stopped() const noexcept;
        size_type pending_tasks() const;
        void await();

        template<class F, class... Args>
        std::future<std::invoke_result_t<F(Args...)>> submit(F&& function, Args&&... args)
        {
            auto task = std::make_shared<std::packaged_task<std::result_of_t<F(Args...)>()>>([=] {
                return function(args...);
            });

            {
                std::unique_lock<std::mutex> l(m_Mutex);

                m_Tasks.emplace([=] {
                    (*task)();
                });
            }

            m_ConditionVariable.notify_one();
            return task->get_future();
        }

        template<class F, class... Args>
        void execute(F&& function, Args&&... args)
        {
            {
                std::unique_lock<std::mutex> l(m_Mutex);

                m_Tasks.emplace([=] {
                    function(args...);
                });
            }

            m_ConditionVariable.notify_one();
        }

    private:
        mutable std::mutex m_Mutex;
        std::condition_variable m_ConditionVariable;
        std::queue<task_type> m_Tasks;
        std::thread m_WorkerThread;
        std::atomic<bool> m_Stopped;
    };
} // namespace cpptp