cmake_minimum_required(VERSION 4.1)

set(CMAKE_EXPERIMENTAL_CXX_IMPORT_STD "d0edc3af-4c50-42ea-a356-e2862fe7a444")
set(CMAKE_CXX_STANDARD 23)
# set(CMAKE_CXX_FLAGS "-stdlib=libc++") # uncomment for Clang

project(cpp_modules_cmake)

add_subdirectory(modules-1)
add_subdirectory(modules-2)
add_subdirectory(modules-3)
# add_subdirectory(drawing-app)
add_subdirectory(import-std/modules-4)
add_subdirectory(import-std/drawing-app)
add_subdirectory(import-std/import-std-lib)