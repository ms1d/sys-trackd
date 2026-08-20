cmake_minimum_required(VERSION 3.26)

project(sys-trackerd LANGUAGES C)

file(GLOB SOURCES src/*.c)

add_executable(sys-trackerd ${SOURCES})
target_include_directories(sys-trackerd PUBLIC include)
target_compile_options(
	sys-trackerd
	PRIVATE
	-g -O0 -Wall -Wextra -Wpedantic -Werror
)
