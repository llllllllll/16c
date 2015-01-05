cmake_minimum_required(VERSION 2.8)
project(16CANDLES)
set(16CANDLES_VERSION_MAJOR 1)
set(16CANDLES_VERSION_MINOR 0)

set(CMAKE_C_FLAGS
    "-g -std=gnu11 -lpthread")

include_directories(include)
add_subdirectory(src)
