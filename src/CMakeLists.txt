#
# cmake for src directory of Felzenswalb segmentation
#
project(SEGMENT_FZ)

cmake_minimum_required(VERSION 2.8)

find_package( OpenCV REQUIRED )

include_directories(.)
set(CMAKE_CXX_FLAGS "-g")
# opencv version
add_executable(fzsegment fzsegment.cpp)
target_link_libraries(fzsegment ${OpenCV_LIBS})
