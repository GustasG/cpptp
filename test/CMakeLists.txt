include(GoogleTest)

FetchContent_Declare(googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG e2239ee6043f73722e7aa812a459f54a28552929 # release v1.11.0
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)

add_executable(cpptp-tests
    worker_test.cpp 
    pool_test.cpp
)

target_link_libraries(cpptp-tests
    cpptp
    gtest
    gtest_main
)

gtest_discover_tests(cpptp-tests)