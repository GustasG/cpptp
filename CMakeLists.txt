cmake_minimum_required(VERSION 3.10)

project(cpptp VERSION 1.0.0 LANGUAGES CXX)

option(CPPTP_ENABLE_EXCEPTIONS "Enable throwing exceptions on errors" ON)

set(CMAKE_CXX_STANDARD 17)

find_package(Threads REQUIRED)

if (CPPTP_ENABLE_EXCEPTIONS)
    add_compile_definitions(CPPTP_ENABLE_EXCEPTIONS)
endif()

file(GLOB CPPTP_FILES
                                include/cpptp/cpptp.hpp
        src/thread_pool.cpp     include/cpptp/thread_pool.hpp
        src/worker.cpp          include/cpptp/worker.hpp)

add_library(${PROJECT_NAME} ${CPPTP_FILES})
target_include_directories(${PROJECT_NAME} PUBLIC include)
target_link_libraries(${PROJECT_NAME} ${CMAKE_THREAD_LIBS_INIT})