cmake_minimum_required(VERSION 3.14)
project(paralgo)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(TBB REQUIRED)

include(FetchContent)

FetchContent_Declare(
        parlaylib
        GIT_REPOSITORY https://github.com/cmuparlay/parlaylib.git
        GIT_TAG        master
)

FetchContent_MakeAvailable(parlaylib)

find_package(Threads REQUIRED)

add_compile_options(-ltbb -DPARALLEL)

add_executable(paralgo main.cpp
        quick_sort.h
        quick_sort.cpp)

target_include_directories(paralgo PRIVATE ${parlaylib_SOURCE_DIR}/include)

target_link_libraries(paralgo PRIVATE
        Threads::Threads
        TBB::tbb
)
