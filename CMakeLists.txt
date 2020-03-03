cmake_minimum_required(VERSION 3.10)
project(ink)

set(CMAKE_CXX_STANDARD 14)
include_directories(/opt/local/include)
add_executable(ink main.cpp)