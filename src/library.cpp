#include "cpptp/cpptr.hpp"

#include <string>
#include <iostream>

using namespace cpptp;

int main() {
    ThreadPool tp;

    std::string s = "First";

    auto t1 = tp.submit([](const std::string& text) {
        std::cout << "Hello, World!" << std::endl;
        std::cout << text << std::endl;

        std::cout << &text << std::endl;

        return 42;
    }, s);

    std::cout << &s << std::endl;
    std::cout << sizeof(Worker) << std::endl;
    std::cout << sizeof(Worker::task_type) << std::endl;
}