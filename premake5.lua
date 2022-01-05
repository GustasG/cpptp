project "cpptp"
    language "C++"
    cppdialect "C++11"
    kind "StaticLib"
    warnings "Everything"
    pic "On"

    includedirs
    {
        "include"
    }

    files
    {
        "include/cpptp/cpptp.hpp",
        "include/cpptp/thread_pool.hpp",
        "include/cpptp/utility.hpp",
        "include/cpptp/worker.hpp",

        "src/thread_pool.cpp",
        "src/worker.cpp"
    }

    defines
    {
        "CPPTP_ENABLE_EXCEPTIONS"
    }

    filter "system:linux"
        links
        {
            "pthread"
        }

    filter "action:vs*"
        buildoptions
        {
            "/Zc:__cplusplus"
        }