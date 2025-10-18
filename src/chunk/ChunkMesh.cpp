//
// Created by Alex on 16/10/2025.
//

#include "ChunkMesh.h"

MinecloneTypes::Vertex vertices[24] = {
    // FRONT (z+)
    {{0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}}, // 0 - bottom left
    {{1.0f, 0.0f, 1.0f}, {1.0f, 0.0f}}, // 1 - bottom right
    {{1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}}, // 2 - top right
    {{0.0f, 1.0f, 1.0f}, {0.0f, 1.0f}}, // 3 - top left

    // BACK (z-)
    {{1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}}, // 4 - bottom left
    {{0.0f, 0.0f, 0.0f}, {1.0f, 0.0f}}, // 5 - bottom right
    {{0.0f, 1.0f, 0.0f}, {1.0f, 1.0f}}, // 6 - top right
    {{1.0f, 1.0f, 0.0f}, {0.0f, 1.0f}}, // 7 - top left

    // LEFT (x-)
    {{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}}, // 8
    {{0.0f, 0.0f, 1.0f}, {1.0f, 0.0f}}, // 9
    {{0.0f, 1.0f, 1.0f}, {1.0f, 1.0f}}, // 10
    {{0.0f, 1.0f, 0.0f}, {0.0f, 1.0f}}, // 11

    // RIGHT (x+)
    {{1.0f, 0.0f, 1.0f}, {0.0f, 0.0f}}, // 12
    {{1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}}, // 13
    {{1.0f, 1.0f, 0.0f}, {1.0f, 1.0f}}, // 14
    {{1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}}, // 15

    // TOP (y+)
    {{0.0f, 1.0f, 1.0f}, {0.0f, 0.0f}}, // 16
    {{1.0f, 1.0f, 1.0f}, {1.0f, 0.0f}}, // 17
    {{1.0f, 1.0f, 0.0f}, {1.0f, 1.0f}}, // 18
    {{0.0f, 1.0f, 0.0f}, {0.0f, 1.0f}}, // 19

    // BOTTOM (y-)
    {{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}}, // 20
    {{1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}}, // 21
    {{1.0f, 0.0f, 1.0f}, {1.0f, 1.0f}}, // 22
    {{0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}}  // 23
};

unsigned int indices[6] = {0,1,2,2,3,0};
//
// ChunkMesh::ChunkMesh(const Chunk &chunkData) {
//     this->chunkData = chunkData;
// }

void ChunkMesh::AddFace(const int face, const int x, const int y, const int z, unsigned int &indexOffset) {
    for (int cubeVertex = 0; cubeVertex < 4; cubeVertex++) {
        MinecloneTypes::Vertex vtx = vertices[face * 4 + cubeVertex];
        vtx.Position.X += x;
        vtx.Position.Y += y;
        vtx.Position.Z += z;
        cubeVertices.push_back(vtx);
    }

    for (int cubeIndex  = 0; cubeIndex < 6; cubeIndex++) {
        cubeIndices.push_back(indices[cubeIndex] + indexOffset);
    }

    indexOffset += 4; // Only increment when we actually add a face
}

void ChunkMesh::BuildMesh() {
    unsigned int indexOffset = 0;

    for (int blockX = 0; blockX < Chunk::SIZE_X; blockX++) {
        for (int blockY = 0; blockY < Chunk::SIZE_Y; blockY++) {
            for (int blockZ = 0; blockZ < Chunk::SIZE_Z; blockZ++) {
                Block currentBlock = chunkData.GetBlockAt(blockX, blockY, blockZ);
                if (currentBlock.GetBlockType() == Block::AIR) continue;

                int worldX = blockX + chunkData.chunkPos.x * Chunk::SIZE_X;
                int worldZ = blockZ + chunkData.chunkPos.y * Chunk::SIZE_Z;

                if (!chunkData.IsBlockSolid(blockX, blockY+1, blockZ)) AddFace(4, worldX, blockY, worldZ, indexOffset);
                if (!chunkData.IsBlockSolid(blockX, blockY-1, blockZ)) AddFace(5, worldX, blockY, worldZ, indexOffset);
                if (!chunkData.IsBlockSolid(blockX+1, blockY, blockZ)) AddFace(3, worldX, blockY, worldZ, indexOffset);
                if (!chunkData.IsBlockSolid(blockX-1, blockY, blockZ)) AddFace(2, worldX, blockY, worldZ, indexOffset);
                if (!chunkData.IsBlockSolid(blockX, blockY, blockZ+1)) AddFace(0, worldX, blockY, worldZ, indexOffset);
                if (!chunkData.IsBlockSolid(blockX, blockY, blockZ-1)) AddFace(1, worldX, blockY, worldZ, indexOffset);
            }
        }
    }

    // VertexArrayObject
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // VertexBufferObject
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, cubeVertices.size() * sizeof(MinecloneTypes::Vertex), cubeVertices.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(MinecloneTypes::Vertex), NULL);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(MinecloneTypes::Vertex), (void *) (3 * sizeof(float)));

    // ElementBufferObjects
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, cubeIndices.size() * sizeof(unsigned int), cubeIndices.data(),
                 GL_STATIC_DRAW);


}

void ChunkMesh::Draw() const {
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, cubeIndices.size(), GL_UNSIGNED_INT, NULL);
}
