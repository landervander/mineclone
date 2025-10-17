//
// Created by Alex on 16/10/2025.
//

#ifndef MINECLONE_CHUNK_H
#define MINECLONE_CHUNK_H
#include <vector>
#include <block/Block.h>

#include "glad/glad.h"

struct Vertex {
    struct Position {
        float X;
        float Y;
        float Z;
    } Position;

    struct UV {
        float x;
        float y;
    } UV;
};

class Chunk {
private:
    static constexpr int SIZE_X = 16;
    static constexpr int SIZE_Y = 256;
    static constexpr int SIZE_Z = 16;

    Block blocks[SIZE_X][SIZE_Y][SIZE_Z] = {};
    GLuint VAO = 0, VBO = 0, EBO = 0;

    std::vector<Vertex> cubeVertices = {};
    std::vector<unsigned int> cubeIndices = {};

    float chunkPos[3] = { 0.f, 0.f, 0.f };

    bool IsBlockSolid(int x, int y, int z);
    void AddFace(int face, int x, int y, int z, unsigned int &indexOffset);

public:
    Chunk(float x, float y, float z);

    void GenerateBlocks();

    void BuildMesh();

    void Draw() const;

    Block GetBlockAt(int x, int y, int z);
};


#endif //MINECLONE_CHUNK_H
