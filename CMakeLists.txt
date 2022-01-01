cmake_minimum_required(VERSION 3.10)

project(cpptp VERSION 1.0.0 LANGUAGES CXX)

option(CPPTP_ENABLE_EXCEPTIONS "Enable exception throwing on errors" ON)
option(CPPTP_ENABLE_TEST_RUNNING "Run project tests" OFF)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

find_package(Threads REQUIRED)

if (CPPTP_ENABLE_EXCEPTIONS)
    add_compile_definitions(CPPTP_ENABLE_EXCEPTIONS)
endif()

if (CPPTP_ENABLE_TEST_RUNNING)
    include(FetchContent)
    FetchContent_Declare(googletest URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip)
    # For Windows: Prevent overriding the parent project's compiler/linker settings
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)

    enable_testing()
    add_subdirectory(tests)
endif()

file(GLOB CPPTP_FILES
                                include/cpptp/cpptp.hpp
        src/thread_pool.cpp     include/cpptp/thread_pool.hpp
        src/worker.cpp          include/cpptp/worker.hpp)

add_library(${PROJECT_NAME} STATIC ${CPPTP_FILES})
target_include_directories(${PROJECT_NAME} PUBLIC include)
target_link_libraries(${PROJECT_NAME} ${CMAKE_THREAD_LIBS_INIT})