cmake_minimum_required(VERSION 3.17)
project(vindecgen)

set(CMAKE_CXX_STANDARD 14)

add_library(vindecgen src/Decoder.cpp src/Decoder.h)

add_executable(testly src/testly.cpp)

