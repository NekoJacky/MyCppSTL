cmake_minimum_required(VERSION 3.22)
project(MyCppSTL)

set(CMAKE_CXX_STANDARD 17)

include_directories(.)

add_executable(MyCppSTL
        main.cpp MyCppSTL/allocator_demo.h)
