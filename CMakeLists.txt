cmake_minimum_required(VERSION 3.17)
project(vindecgen)

set(CMAKE_CXX_STANDARD 14)

add_library(vindec SHARED include/VinDecoder.h src/VinDecoder.cpp)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/resources/vindec_resource_countries.txt
        ${CMAKE_BINARY_DIR} COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/resources/vindec_resource_manufacturers.txt
        ${CMAKE_BINARY_DIR} COPYONLY)