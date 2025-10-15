cmake_minimum_required(VERSION 4.0)

add_library(stb
        image/stb_image.cpp
)

target_include_directories(stb PUBLIC image)