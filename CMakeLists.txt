cmake_minimum_required(VERSION 3.10)
project(ink)

set(CMAKE_CXX_STANDARD 14)
include_directories(/opt/local/include)
link_directories(/opt/local/lib)
link_libraries(-lgtest -pthread)
add_executable(ink.out main.cpp main_msgpack.cpp main_websocketpp.cpp websocketpp_client.cpp tests.cpp CapFile.cpp misc.cpp misc.hpp FileSet.cpp FileSet.hpp parser.cpp misc.cpp Id.cpp Message.cpp Message.hpp)