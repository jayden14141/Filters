add_executable(benchmark_executable benchmark.cpp
        ../src/CSVWriter.h
        ../src/CSVWriter.cpp)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -Wall -Wextra -march=native -funroll-loops")

target_link_libraries(benchmark_executable filters)