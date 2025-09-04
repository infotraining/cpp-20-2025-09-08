cmake_minimum_required(VERSION 3.28)

project(primes)

set(CMAKE_CXX_STANDARD 20)

add_library(primes1_lib)

target_sources(primes1_lib
  PUBLIC
    FILE_SET CXX_MODULES FILES
    primes.cxx
)

add_executable(primes1 primes_main.cpp)
target_link_libraries(primes1 PRIVATE primes1_lib)