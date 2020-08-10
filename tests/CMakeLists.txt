cmake_minimum_required(VERSION 3.17)
set(CMAKE_CXX_STANDARD 14)

set(SOURCES VinDecoderTest.cpp)
add_executable(vin_decoder_test ${SOURCES})

target_link_libraries(vin_decoder_test PUBLIC
    gtest_main
    vindec
)

add_test(NAME vin_decoder_test COMMAND vin_decoder_test)