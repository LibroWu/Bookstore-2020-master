cmake_minimum_required(VERSION 3.17)
project(src)

set(CMAKE_CXX_STANDARD 14)

add_executable(src main.cpp Apollo.cpp user.cpp Unrolled_Linked_List.hpp)