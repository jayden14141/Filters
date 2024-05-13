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
        src/cuckoo.h
        src/cuckoo.cpp
        src/xor.h
        src/xor.cpp
        src/blocked_bloom.h
        src/blocked_bloom.cpp
        src/cuckoo_test.cpp
        src/cuckoo_test.h
        src/table.h
        src/blocked_bloom_test.h
        src/blocked_bloom_test.cpp
        src/cuckoo_test_fingerprint.h
        src/cuckoo_test_fingerprint.cpp
        src/xor_test.h
        src/xor_test.cpp
)

if (MSVC)
    # warning level 4
    add_compile_options(/W4)
else()
    # additional warnings
    add_compile_options(-DNDEBUG -O3 -march=native -fprefetch-loop-arrays)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
endif()
