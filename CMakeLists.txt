cmake_minimum_required(VERSION 3.10)
project(interprete C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Incluimos todos los archivos .c dentro de src y sus subdirectorios
file(GLOB_RECURSE SRCS "src/*.c")

# Creamos el ejecutable
add_executable(interprete ${SRCS})

# Librerías (math)
target_link_libraries(interprete m)

# Opciones de compilación
target_compile_options(interprete PRIVATE -Wall -Wextra)
