cmake_minimum_required(VERSION 3.10)
project(ink)

set(CMAKE_CXX_STANDARD 14)
include_directories(/opt/local/include)
link_directories(/opt/local/lib)
link_libraries(-lgtest -pthread)
add_executable(ink.out main.cpp main_msgpack.cpp main_websocketpp.cpp websocketpp_client.cpp CapFile.cpp misc.cpp FileSet.cpp decoder.cpp misc.cpp Id.cpp Message.cpp rows.cpp Store.cpp String.cpp)