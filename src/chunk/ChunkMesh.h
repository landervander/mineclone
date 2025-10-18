//
// Created by Alex on 16/10/2025.
//

#ifndef MINECLONE_CHUNKMESH_H
#define MINECLONE_CHUNKMESH_H
#include <vector>

#include <glad/glad.h>

#include "Chunk.h"
#include <types/Types.h>

class ChunkMesh {
public:
    explicit ChunkMesh(const Chunk &chunkData) : chunkData(chunkData) {};

    void BuildMesh();

    void Draw() const;

private:
    GLuint VAO = 0, VBO = 0, EBO = 0;
    GLuint indexCount = 0;

    std::vector<MinecloneTypes::Vertex> cubeVertices = {};
    std::vector<unsigned int> cubeIndices = {};

    const Chunk& chunkData;

    void AddFace(int face, int x, int y, int z, unsigned int &indexOffset);
};


#endif //MINECLONE_CHUNKMESH_H