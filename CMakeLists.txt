cmake_minimum_required(VERSION 3.26)

project(sys-trackerd LANGUAGES C)

file(GLOB D_SOURCES src/daemon/*.c)
file(GLOB C_SOURCES src/client/*.c)

add_executable(sys-trackerd ${D_SOURCES})
add_executable(sys-tracker ${C_SOURCES})
target_include_directories(sys-trackerd PUBLIC include)
target_compile_options(
	sys-trackerd
	PRIVATE
	-g -O3 -Wall -Wextra -Wpedantic -Werror
)

target_include_directories(sys-tracker PUBLIC include)
target_compile_options(
	sys-trackerd
	PRIVATE
	-g -O3 -Wall -Wextra -Wpedantic -Werror
)
