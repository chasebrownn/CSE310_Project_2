cmake_minimum_required(VERSION 2.8.4)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

project(Code)

set(CMAKE_CXX_STANDARD 14)

add_executable(Code myAppStore.cpp)