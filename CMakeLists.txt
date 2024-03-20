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
        src/util.h
        src/util.cpp
        src/table.cpp
        src/table.h
        src/cuckoo.h
        src/cuckoo.cpp
        src/xorTable.h
        src/xorTable.cpp
        #        src/xor.h
#        src/xor.cpp
)

if (MSVC)
    # warning level 4
    add_compile_options(/W4)
else()
    # additional warnings
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()
