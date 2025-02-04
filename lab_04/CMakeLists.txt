cmake_minimum_required(VERSION 3.10)
project(DynamicLibExample)

set(CMAKE_CXX_STANDARD 14)

# Создаём динамическую библиотеку (libsolution.so)
add_library(solution_shared SHARED solution.cpp)
set_target_properties(solution_shared PROPERTIES OUTPUT_NAME "solution")

# Создаём статическую библиотеку (libsolution.a)
add_library(solution_static STATIC solution.cpp)

# Главная программа (linking к статической или динамической библиотеке)
add_executable(program_static main.cpp)
target_link_libraries(program_static solution_static)

add_executable(program_dynamic main.cpp)
target_link_libraries(program_dynamic solution_shared)