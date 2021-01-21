cmake_minimum_required(VERSION 3.17)
project(src)

set(CMAKE_CXX_STANDARD 14)

add_executable(src main.cpp error.cpp user.cpp Apollo.cpp Unrolled_Linked_List.hpp Watcher.cpp)