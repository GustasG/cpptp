# cpptp

[![CodeQL](https://github.com/GustasG/cpptp/actions/workflows/codeql-analysis.yml/badge.svg)](https://github.com/GustasG/cpptp/actions/workflows/codeql-analysis.yml)
[![Codacy Badge](https://app.codacy.com/project/badge/Grade/ecea03ac10524e198bcb5cb74e17cc57)](https://www.codacy.com/gh/GustasG/cpptp)
[![Language grade: C/C++](https://img.shields.io/lgtm/grade/cpp/g/GustasG/cpptp.svg?logo=lgtm&logoWidth=18)](https://lgtm.com/projects/g/GustasG/cpptp/context:cpp)
[![Total alerts](https://img.shields.io/lgtm/alerts/g/GustasG/cpptp.svg?logo=lgtm&logoWidth=18)](https://lgtm.com/projects/g/GustasG/cpptp/alerts/)
[![GitHub license](https://img.shields.io/badge/license-MIT-blue.svg)](https://raw.githubusercontent.com/GustasG/cpptp/master/LICENSE)
[![FOSSA Status](https://app.fossa.com/api/projects/git%2Bgithub.com%2FGustasG%2Fcpptp.svg?type=shield)](https://app.fossa.com/projects/git%2Bgithub.com%2FGustasG%2Fcpptp?ref=badge_shield)

A simple and easy to use C++17 thread pool implementation

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

## License

[![FOSSA Status](https://app.fossa.com/api/projects/git%2Bgithub.com%2FGustasG%2Fcpptp.svg?type=large)](https://app.fossa.com/projects/git%2Bgithub.com%2FGustasG%2Fcpptp?ref=badge_large)
