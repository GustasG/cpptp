cmake_minimum_required(VERSION 3.11)

project(cpptp
    VERSION 1.0.0
    DESCRIPTION "C++ Thread Pool"
    HOMEPAGE_URL "https://github.com/GustasG/cpptp"
    LANGUAGES CXX
)

option(CPPTP_RUN_TESTS "Run tests" OFF)
option(CPPTP_DISABLE_EXCEPTIONS "Disable exception throwing" OFF)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

find_package(Threads REQUIRED)

include(FetchContent)

add_subdirectory(libs/concurrentqueue)

if (CPPTP_DISABLE_EXCEPTIONS)
    target_compile_definitions(${PROJECT_NAME} CPPTP_DISABLE_EXCEPTIONS)
endif()

add_library(${PROJECT_NAME} STATIC
    include/cpptp/cpptp.hpp
    include/cpptp/thread_pool.hpp   src/thread_pool.cpp
    include/cpptp/worker.hpp        src/worker.cpp
)

target_include_directories(${PROJECT_NAME} PUBLIC include)

target_link_libraries(${PROJECT_NAME}
    ${CMAKE_THREAD_LIBS_INIT}
    concurrentqueue
)

if (CPPTP_RUN_TESTS)
    enable_testing()
    add_subdirectory(test)
endif()