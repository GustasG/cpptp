# cpptp

A simple and easy to use C++11 thread pool implementation

Simple usage example:

```c++
// Create thread pool with available number of processors
cpptp::ThreadPool pool;

// Submit task to pool for execution and return future for executed result
auto t1 = pool.submit([] {
   std::this_thread::sleep_for(std::chrono::seconds(2));
   return 20;
});

// 20 will be printed to the console
std::cout << t1.get() << std::endl;
```