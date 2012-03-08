#
# cmake for src directory of Felzenswalb segmentation
#
project(SEGMENT_FZ)

cmake_minimum_required(VERSION 2.8)

find_package( OpenCV REQUIRED )

include_directories(.)
set(CMAKE_CXX_FLAGS "-g -O3")
add_executable(segment fzsegment.cpp)
target_link_libraries(segment ${OpenCV_LIBS})
