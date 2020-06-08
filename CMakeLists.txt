cmake_minimum_required(VERSION 3.10)
project(ink)

set(CMAKE_CXX_STANDARD 14)
set (CMAKE_CXX_FLAGS "-fno-exceptions -fno-rtti")
include_directories(/opt/local/include)
link_directories(/opt/local/lib)
link_libraries(-lgtest -pthread)
add_executable(ink.out main.cpp main_websocketpp.cpp websocketpp_client.cpp CapFile.cpp misc.cpp
        FileSet.cpp Message.cpp misc.cpp Id.cpp rows.cpp Store.cpp)