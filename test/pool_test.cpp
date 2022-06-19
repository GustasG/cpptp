#include <chrono>
#include <exception>

#include <gtest/gtest.h>

#include <cpptp/thread_pool.hpp>

using namespace std::literals::chrono_literals;


TEST(PoolTest, CreatingPoolWithAvailableProcessorCount) {
    cpptp::ThreadPool pool;

    ASSERT_EQ(pool.worker_count(), std::thread::hardware_concurrency());
}

TEST(PoolTest, CreatePoolWithGivenNumberOfWorkers) {
    cpptp::ThreadPool pool(2);

    ASSERT_EQ(pool.worker_count(), 2);
}

TEST(PoolTest, CreateThreadWithZeroWorkersAndCheckThatAtLeastOneThreadWasCreated) {
    cpptp::ThreadPool pool(0);

    ASSERT_EQ(pool.worker_count(), 1);
}

TEST(PoolTest, UsePoolsWorkerExplicityly) {
    cpptp::ThreadPool pool(2);
    auto worker = pool.acquire_worker();
    std::thread::id id1, id2;

    worker->execute([&] {
        id1 = std::this_thread::get_id();
    });

    worker->execute([&] {
        id2 = std::this_thread::get_id();
    });

    worker->wait();

    ASSERT_EQ(id1, id2);
}

TEST(PoolTest, CheckingPendingTasksInsideEmptyPool) {
    cpptp::ThreadPool pool;

    ASSERT_EQ(pool.pending_task_count(), 0);
}

TEST(PoolTest, SubmitingGivenTaskToPool) {
    cpptp::ThreadPool pool;
    auto future = pool.submit([] {
        return 50;
    });

    ASSERT_EQ(future.get(), 50);
}

TEST(PoolTest, SubmitingTasksToPoolThatHasBeenStopped) {
    cpptp::ThreadPool pool;
    pool.stop();

    ASSERT_THROW({
        pool.submit([] {
            std::this_thread::sleep_for(1s);
        });
    }, std::runtime_error);
}