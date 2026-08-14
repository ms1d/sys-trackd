cmake_minimum_required(VERSION 3.26)

project(overlay LANGUAGES CXX)

file(GLOB SOURCES src/*.cpp)
add_executable(overlay ${SOURCES})
