cmake_minimum_required(VERSION 3.22)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_CXX_EXTENSIONS FALSE)

set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wpedantic -Werror -fcoroutines")

project(generator)

add_executable(generator main.cpp)
