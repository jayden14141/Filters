cmake_minimum_required(VERSION 3.26)
project(filters)

set(CMAKE_CXX_STANDARD 17)

add_library(filters library.cpp
        src/bloom.cpp
        src/bloom.h
        hash_function.cpp
        hash_function.h
)
