#include <string>
#include <iostream>

#include "cpptp/cpptr.hpp"

static bool is_prime(size_t number) noexcept
{
    for (size_t i = 2; i <= static_cast<size_t>(sqrt(number)); i++)
    {
        if (number % i == 0)
            return false;
    }

    return true;
}

using namespace cpptp;

int main() {
    ThreadPool tp;

    std::vector<int> numbers {1, 2, 3, 4, 5};

    tp.for_each(numbers.begin(), numbers.end(), [] (int& value){
       value *= 2;
    });

    tp.await();

    std::cout << tp.pending_tasks() << std::endl;

    for (const auto& number : numbers)
    {
        std::cout << number << std::endl;
    }

//    std::string m = "asas";
//
//    tp.execute([] (const std::string& v){
//       std::cout << "Worker "<< &v << std::endl;
//    }, m);
//
//    tp.await();
//
//    std::cout << &m << std::endl;
}