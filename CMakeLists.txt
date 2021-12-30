cmake_minimum_required(VERSION 3.21)
project(cpptp)

set(CMAKE_CXX_STANDARD 11)

include_directories(include)
include_directories(src)

file(GLOB SRC_FILES
        src/library.cpp
        src/worker.cpp          include/cpptp/worker.hpp
        src/thread_pool.cpp     include/cpptp/thread_pool.hpp
                                include/cpptp/detail/mutex.hpp)

add_executable(${PROJECT_NAME} ${SRC_FILES})