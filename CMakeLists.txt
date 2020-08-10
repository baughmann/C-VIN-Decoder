cmake_minimum_required(VERSION 3.17)
project(vindecgen)

set(CMAKE_CXX_STANDARD 14)

add_library(vindec SHARED include/Decoder.h src/Decoder.cpp)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/resources/countries.txt
        ${CMAKE_BINARY_DIR} COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/resources/manufacturers.txt
        ${CMAKE_BINARY_DIR} COPYONLY)