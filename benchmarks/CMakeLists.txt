add_executable(benchmark_executable benchmark.cpp)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -march=native -funroll-loops")

target_link_libraries(benchmark_executable filters)