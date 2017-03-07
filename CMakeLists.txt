cmake_minimum_required(VERSION 3.6)
project(k_means_seed_data)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})

set(SOURCE_FILES main.cpp)
add_executable(k_means_seed_data ${SOURCE_FILES})