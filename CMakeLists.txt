cmake_minimum_required(VERSION 3.10)
project(resistor)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_BUILD_TYPE Debug)

include_directories(include)

file(GLOB SOURCES "src/*.cpp")
add_executable(resistor ${SOURCES})
