cmake_minimum_required(VERSION 3.26)

project(overlay LANGUAGES C)

file(GLOB SOURCES src/*.c)
add_executable(overlay ${SOURCES})
