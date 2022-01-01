#include <exception>

#include <gtest/gtest.h>

#include <cpptp/worker.hpp>

TEST(WorkerTest, BasicWorkerCreationTest) {
    EXPECT_NO_THROW({
        cpptp::Worker worker;
    });
}

TEST(WorkerTest, WorkerExecutionTest) {
    cpptp::Worker worker;
    int value = 50;

    worker.execute([&]{
        value -= 10;
    });

    worker.await();

    ASSERT_EQ(value, 40);
}

TEST(WorkerTest, WorkerExecutionTestThatThrowsException) {
    cpptp::Worker worker;

    EXPECT_NO_THROW({
        worker.execute([]{
            throw std::runtime_error("Example exception");
        });

        worker.await();
    });
}

TEST(WorkerTest, WorkerExecutionAfterStop) {
    cpptp::Worker worker;
    worker.stop();

    ASSERT_THROW({
        worker.execute([] {
            std::this_thread::sleep_for(std::chrono::seconds(1));
        });
    }, std::runtime_error);
}

TEST(WorkerTest, WorkerSubmitionTestThatThrowsException) {
    cpptp::Worker worker;

    auto future = worker.submit([] {
       throw std::runtime_error("Example exception");
    });

    EXPECT_THROW(future.get(), std::runtime_error);
}

TEST(WorkerTest, WorkerSubmitionAndPendingTasksRetrieval) {
    cpptp::Worker worker;

    auto future = worker.submit([] {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    });

    future.get();

    ASSERT_EQ(worker.pending_tasks(), 0);
}

TEST(WorkerTest, WorkerSubmitionAfterStop) {
    cpptp::Worker worker;
    worker.stop();

    ASSERT_THROW({
        worker.submit([] {
            std::this_thread::sleep_for(std::chrono::seconds(1));
        });
    }, std::runtime_error);
}

TEST(WorkerTest, WorkerStopingProcedure) {
    cpptp::Worker worker;
    worker.stop();

    ASSERT_TRUE(worker.stopped());
}