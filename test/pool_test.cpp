#include <vector>
#include <exception>

#include <gtest/gtest.h>

#include <cpptp/thread_pool.hpp>

TEST(PoolTest, CreatingPoolWithAvailableProcessorCount) {
    cpptp::ThreadPool pool;

    ASSERT_EQ(pool.workers(), std::thread::hardware_concurrency());
}

TEST(PoolTest, CreatePoolWithGivenNumberOfWorkers) {
    cpptp::ThreadPool pool(2);

    ASSERT_EQ(pool.workers(), 2);
}

TEST(PoolTest, CreateThreadWithZeroWorkersAndCheckThatAtLeastOneThreadWasCreated) {
    cpptp::ThreadPool pool(0);

    ASSERT_EQ(pool.workers(), 1);
}

TEST(PoolTest, UsePoolsWorkerExplicityly) {
    cpptp::ThreadPool pool(2);
    auto worker = pool.acquire_worker();
    std::thread::id id1, id2;

    worker->execute([&]{
        id1 = std::this_thread::get_id();
    });

    worker->execute([&]{
        id2 = std::this_thread::get_id();
    });

    pool.await();

    ASSERT_EQ(id1, id2);
}

TEST(PoolTest, CheckingPendingTasksInsideEmptyPool) {
    cpptp::ThreadPool pool;

    ASSERT_EQ(pool.pending_tasks(), 0);
}

TEST(PoolTest, SubmitingGivenTaskToPool) {
    cpptp::ThreadPool pool;
    auto future = pool.submit([] {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        return 50;
    });

    ASSERT_EQ(future.get(), 50);
}

TEST(PoolTest, SubmitingTasksToPoolThatHasBeenStopped) {
    cpptp::ThreadPool pool;
    pool.stop();

    ASSERT_THROW({
        pool.submit([] {
            std::this_thread::sleep_for(std::chrono::seconds(1));
        });
    }, std::runtime_error);
}

TEST(PoolTest, PassingIteratorToForEachMethod) {
    cpptp::ThreadPool pool;

    std::vector<int> values {1, 3, 5};

    pool.for_each(values.begin(), values.end(), [](int& number) {
       number *= 10;
    });

    pool.await();

    std::vector<int> expected{ 10, 30, 50 };

    ASSERT_EQ(values, expected);
}