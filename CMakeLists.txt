cmake_minimum_required(VERSION 3.22)

project(MyCppSTL)

set(CMAKE_CXX_STANDARD 17)

include_directories(.)

aux_source_directory(. SRCS)

add_executable(MyCppSTL ${SRCS} test.h)

