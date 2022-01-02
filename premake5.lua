project "cpptp"
    language "C++"
    cppdialect "C++11"
    kind "StaticLib"
    warnings "Everything"

    includedirs
    {
        "include"
    }

    files
    {
        "include/cpptp/cpptp.hpp",
        "include/cpptp/thread_pool.hpp",
        "include/cpptp/worker.hpp",

        "src/thread_pool.cpp",
        "src/worker.cpp"
    }

    filter "system:linux"
        links
        {
            "pthread"
        }