cmake_minimum_required(VERSION 3.12)
project(Filter)

set(CMAKE_CXX_STANDARD 14)

add_executable(Filter main.cpp)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
target_link_libraries(Filter ${OpenCV_LIBS})