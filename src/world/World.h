//
// Created by Alex on 17/10/2025.
//

#ifndef MINECLONE_WORLD_H
#define MINECLONE_WORLD_H

#include <queue>
#include <chunk/Chunk.h>
#include <chunk/ChunkMesh.h>
#include <atomic>
#include <mutex>

class World {
public:
    World();

    void GenerateWorld();
    void Update();
    void RenderUI();
    void RenderWorld();
    Chunk* GetChunkAt(int x, int y) const;

    glm::vec3 playerPosition = {};
private:
    int viewDistance = 8;
    // std::unordered_map<Chunk, ChunkMesh, ChunkHasher> chunks = {};
    std::vector<Chunk*> chunks = {}; // Logical Chunks
    std::queue<ChunkMesh*> chunkMeshQueue = {}; // Build Mesh
    std::vector<ChunkMesh*> chunksToRender = {}; // Render Mesh

    std::atomic<bool> running;

    void GenerationThread();
};


#endif //MINECLONE_WORLD_H