cmake_minimum_required(VERSION 3.17)
set(CMAKE_CXX_STANDARD 14)
project(vindec 
        VERSION 0.1 
        DESCRIPTION "Simple utility to decode and validate Vehicle Identification Numbers"
        HOMEPAGE_URL "https://github.com/baughmann/Cpp-VIN-Decoder"
        )

enable_testing()
include(GoogleTest)
add_subdirectory(ThirdParty/google/googletest)
add_subdirectory(tests)

set(HEADERS include/VinDecoder.h)
set(SOURCES src/VinDecoder.cpp)

add_library(vindec STATIC ${HEADERS} ${SOURCES})
target_include_directories(vindec PUBLIC ${PROJECT_SOURCE_DIR}/include)