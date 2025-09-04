cmake_minimum_required(VERSION 4.1)

set(CMAKE_EXPERIMENTAL_CXX_IMPORT_STD "d0edc3af-4c50-42ea-a356-e2862fe7a444")
set(CMAKE_CXX_MODULE_STD 1)

project(drawing-app LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 23)

add_library(singleton_lib)

target_sources(singleton_lib
  PUBLIC
    FILE_SET CXX_MODULES FILES
    Singleton.cxx
)

add_library(factory_lib)

target_sources(factory_lib
  PUBLIC
    FILE_SET CXX_MODULES FILES
    Factory.cxx
)

target_link_libraries(factory_lib PUBLIC singleton_lib)

add_library(drawing_lib)

target_sources(drawing_lib
  PUBLIC
    FILE_SET CXX_MODULES FILES
    Shapes.cxx
    Shape-Factory.cxx
    Shapes-Point.cxx
    Shapes-Base.cxx
    Shapes-Square.cxx
    Shapes-Rectangle.cxx
)

target_link_libraries(drawing_lib PUBLIC factory_lib)

add_executable(drawing_app DrawingApp.cpp)
target_link_libraries(drawing_app PRIVATE drawing_lib)