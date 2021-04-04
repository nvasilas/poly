cmake_minimum_required(VERSION 3.16)
project(poly_benchmark LANGUAGES CXX)

set(CMAKE_BUILD_TYPE Release CACHE STRING "Set build type." FORCE)

find_package(benchmark REQUIRED)

add_library(benchmark_options INTERFACE)
target_compile_features(benchmark_options INTERFACE cxx_std_17)

find_package(blaze REQUIRED)
set(BENCHMARK_FLAGS
    -march=x86-64
    -mtune=generic
    -mavx
    -O3
    -pipe
    -Wall
    -pedantic
    -Werror
    -fno-rtti
)
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR
	CMAKE_CXX_COMPILER_ID MATCHES ".*Clang")
    target_compile_options(benchmark_options INTERFACE ${BENCHMARK_FLAGS})
else()
    message(AUTHOR_WARNING
	"No compiler warnings set for '${CMAKE_CXX_COMPILER_ID}' compiler.")
endif()

add_executable(benchmark_transform_vs_loop benchmark_transform_vs_loop.cpp)
target_link_libraries(benchmark_transform_vs_loop
    benchmark_options benchmark::benchmark)

add_executable(benchmark_add_overloads benchmark_add_overloads.cpp)
target_link_libraries(benchmark_add_overloads
    benchmark_options benchmark::benchmark)

add_executable(benchmark_to_coeff_matrix benchmark_to_coeff_matrix.cpp)
target_link_libraries(benchmark_to_coeff_matrix
    benchmark_options benchmark::benchmark blaze::blaze)
