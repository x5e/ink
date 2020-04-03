cmake_minimum_required(VERSION 3.10)
project(ink)

set(CMAKE_CXX_STANDARD 14)
include_directories(/opt/local/include)
link_directories(/opt/local/lib)
link_libraries(-lgtest)
add_executable(ink main.cpp main_msgpack.cpp main_websocketpp.cpp websocketpp_client.cpp)