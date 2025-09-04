cmake_minimum_required(VERSION 3.28)

project(primes)

set(CMAKE_CXX_STANDARD 20)

add_library(primes2_lib)

target_sources(primes2_lib
  PUBLIC
    FILE_SET CXX_MODULES FILES
    primes.cxx
)

target_sources(primes2_lib 
  PRIVATE
    primes.cpp
)

add_executable(primes2 primes_main.cpp)
target_link_libraries(primes2 PRIVATE primes2_lib)