cmake_minimum_required(VERSION 3.5.1)
project(Concepts)

find_package(Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(main main.cpp)
