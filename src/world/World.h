//
// Created by Alex on 17/10/2025.
//

#ifndef MINECLONE_WORLD_H
#define MINECLONE_WORLD_H

#include <queue>
#include <chunk/Chunk.h>
#include <chunk/ChunkMesh.h>

class World {
public:
    World();

    void GenerateWorld();
    void Update();
    void RenderWorld() const;

    glm::vec3 playerPosition = {};
private:
    int viewDistance = 4;
    // std::unordered_map<Chunk, ChunkMesh, ChunkHasher> chunks = {};
    std::vector<std::pair<Chunk, ChunkMesh>> chunks = {};
    std::queue<ChunkMesh> meshQueue = {};

    void GenerationThread();
};


#endif //MINECLONE_WORLD_H