#
# cmake for src directory of Felzenswalb segmentation
#
project(SEGMENT_FZ)

cmake_minimum_required(VERSION 2.8)

include_directories(.)
set(CMAKE_CXX_FLAGS "-g -O3")
add_executable(segment segment.cpp)
