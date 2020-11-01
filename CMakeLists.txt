cmake_minimum_required(VERSION 3.10)
project(ink)

set(CMAKE_CXX_STANDARD 14)
set (CMAKE_CXX_FLAGS "-fno-exceptions -fno-rtti")
include_directories(/opt/local/include)
link_directories(/opt/local/lib)
link_libraries(-lgtest -pthread)
add_executable(ink.out cpp_files/main.cpp cpp_files/CapFile.cpp cpp_files/misc.cpp
        cpp_files/FileSet.cpp cpp_files/Decoder.cpp cpp_files/misc.cpp cpp_files/Id.cpp cpp_files/rows.cpp cpp_files/Store.cpp)