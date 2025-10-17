cmake_minimum_required(VERSION 4.0)
project(mineclone)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(lib/glfw)
add_subdirectory(lib/glad)
add_subdirectory(lib/glm)
add_subdirectory(lib/stb)


add_executable(mineclone
        src/main.cpp
        src/block/Block.cpp
        src/block/Block.h
        src/shader/shader.cpp
        src/shader/shader.h
        src/chunk/Chunk.cpp
        src/chunk/Chunk.h
        src/chunk/ChunkMesh.cpp
        src/chunk/ChunkMesh.h
        src/world/World.cpp
        src/world/World.h
)

target_include_directories(mineclone PRIVATE src)

target_link_libraries(mineclone PRIVATE
        glfw
        glad
        opengl32
        glm
        stb
)
