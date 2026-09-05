cmake_minimum_required(VERSION 3.26)

project(sys-trackd LANGUAGES C)

file(GLOB D_SOURCES src/daemon/*.c)
file(GLOB C_SOURCES src/client/*.c)

add_executable(sys-trackd ${D_SOURCES})
add_executable(sys-track ${C_SOURCES})
target_include_directories(sys-trackd PUBLIC include)
target_compile_options(
	sys-trackd
	PRIVATE
	-g -O3 -Wall -Wextra -Wpedantic -Werror
)

target_include_directories(sys-track PUBLIC include)
target_compile_options(
	sys-trackd
	PRIVATE
	-g -O3 -Wall -Wextra -Wpedantic -Werror
)
