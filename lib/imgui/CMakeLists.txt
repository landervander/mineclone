cmake_minimum_required(VERSION 4.0)

add_library(imgui
        imgui.cpp
        imgui_demo.cpp
        imgui_draw.cpp
        imgui_tables.cpp
        imgui_widgets.cpp

        imgui_impl_glfw.cpp
        imgui_impl_opengl3.cpp
)

target_link_libraries(imgui PRIVATE glfw)

target_include_directories(imgui PUBLIC .)