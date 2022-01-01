cmake_minimum_required(VERSION 3.10)

project(cpptp VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)

find_package (Threads REQUIRED)

file(GLOB CPPTP_FILES
                                include/cpptp/cpptp.hpp
        src/thread_pool.cpp     include/cpptp/thread_pool.hpp
        src/worker.cpp          include/cpptp/worker.hpp)

add_library(${PROJECT_NAME} ${CPPTP_FILES})
target_include_directories(${PROJECT_NAME} PUBLIC include)
target_link_libraries(${PROJECT_NAME}  ${CMAKE_THREAD_LIBS_INIT})