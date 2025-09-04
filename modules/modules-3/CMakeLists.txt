cmake_minimum_required(VERSION 3.28)

project(primes)

set(CMAKE_CXX_STANDARD 20)

add_library(math_lib)

target_sources(math_lib
  PUBLIC
    FILE_SET CXX_MODULES FILES
    math.cxx
    primes.cxx
    fibonacci_seq.cxx
)

add_executable(math math_main.cpp)
target_link_libraries(math PRIVATE math_lib)