cmake_minimum_required(VERSION 3.10)
project(interprete C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

file(GLOB SRCS "*.c")

add_executable(interprete ${SRCS})
target_link_libraries(interprete m)
target_compile_options(interprete PRIVATE -Wall -Wextra)
