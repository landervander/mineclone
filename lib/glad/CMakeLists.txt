cmake_minimum_required(VERSION 4.0)

add_library(glad
    src/glad.c
)

target_include_directories(glad PUBLIC include)