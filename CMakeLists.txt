cmake_minimum_required(VERSION 3.26)
project(filters)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(tests)
add_subdirectory(benchmarks)

add_library(filters
        src/bloom.cpp
        src/bloom.h
        src/hash_function.cpp
        src/hash_function.h
)
