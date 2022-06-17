include(GoogleTest)

add_executable(cpptp-tests
    worker_test.cpp 
    pool_test.cpp
)

target_link_libraries(cpptp-tests gtest gtest_main cpptp)

gtest_discover_tests(cpptp-tests)