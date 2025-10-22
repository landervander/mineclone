//
// Created by Alex on 16/10/2025.
//

#include "Chunk.h"

void Chunk::SetChunkPosition(int x, int y) {
    chunkPos.x = x;
    chunkPos.y = y;
}

glm::vec2 Chunk::GetChunkPosition() const {
    return this->chunkPos;
}

void Chunk::GenerateBlocks() {
    for (int x = 0; x < SIZE_X; x++) {
        for (int y = 0; y < SIZE_Y; y++) {
            for (int z = 0; z < SIZE_Z; z++) {
                blocks[x][y][z] = Block(Block::DIRT);
            }
        }
    }
}

Block Chunk::GetBlockAt(int x, int y, int z) const {
    return blocks[x][y][z];
}

bool Chunk::IsBlockSolid(int x, int y, int z) const {
    // Return false if outside the chunk bounds
    if (x < 0 || x >= SIZE_X ||
        y < 0 || y >= SIZE_Y ||
        z < 0 || z >= SIZE_Z)
        return false; // treat out-of-bounds as air (so border faces render)

    return blocks[x][y][z].GetBlockType() != Block::AIR;
}