cmake_minimum_required(VERSION 3.28)
project(lab3 C)

set(CMAKE_C_STANDARD 17)

add_executable(lab3
        main.c
        SystemErrors/errors.c)
add_executable(ex child.c
        child.c)
