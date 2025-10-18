//
// Created by Alex on 16/10/2025.
//

#ifndef MINECLONE_CHUNK_H
#define MINECLONE_CHUNK_H

#include <block/Block.h>
#include <glm/glm.hpp>

class Chunk {
public:
    static constexpr int SIZE_X = 16;
    static constexpr int SIZE_Y = 256;
    static constexpr int SIZE_Z = 16;

    glm::ivec2 chunkPos = {};

    Chunk() = default;

    void SetChunkPosition(int x, int y);
    void GenerateBlocks();

    [[nodiscard]] glm::vec2 GetChunkPosition() const;
    [[nodiscard]] Block GetBlockAt(int x, int y, int z) const;
    [[nodiscard]] bool IsBlockSolid(int x, int y, int z) const;

    bool operator==(const Chunk& other) const {
        return chunkPos.x == other.chunkPos.x && chunkPos.y == other.chunkPos.y;
    }

private:
    Block blocks[SIZE_X][SIZE_Y][SIZE_Z] = {};
};

class ChunkHasher {
    public:
    std::size_t operator()(const Chunk& chunk) const {
        return std::hash<int>()(chunk.chunkPos.x ^ (std::hash<int>()(chunk.chunkPos.y) << 1));
    }
};


#endif //MINECLONE_CHUNK_H
