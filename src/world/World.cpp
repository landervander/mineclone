//
// Created by Alex on 17/10/2025.
//

#include "World.h"

#include <thread>

World::World() {
    // printf("World thread started\n");
    // std::thread worldGen(&World::GenerateWorld, this);
    // worldGen.detach();
}

void World::GenerateWorld() {
    for (int x = -viewDistance; x < viewDistance; x++) {
        for (int y = -viewDistance; y < viewDistance; y++) {
            Chunk chunk;
            chunk.SetChunkPosition(x, y);
            chunk.GenerateBlocks();

            ChunkMesh chunkMesh(chunk);
            chunkMesh.BuildMesh();

            // ChunkData

            this->chunks.emplace_back(chunk, chunkMesh);
        }
    }
}

void World::Update() {
    // if (!this->meshQueue.empty()) {
    //     const auto &meshData = this->meshQueue.front();
    //
    //     GLuint VAO = 0, VBO = 0, EBO = 0;
    //
    //     // VertexArrayObject
    //     glGenVertexArrays(1, &VAO);
    //     glBindVertexArray(VAO);
    //
    //     // VertexBufferObject
    //     glGenBuffers(1, &VBO);
    //     glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //     glBufferData(GL_ARRAY_BUFFER, meshData.vertices.size() * sizeof(MinecloneTypes::Vertex), meshData.vertices.data(), GL_STATIC_DRAW);
    //
    //     glEnableVertexAttribArray(0);
    //     glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(MinecloneTypes::Vertex), NULL);
    //     glEnableVertexAttribArray(1);
    //     glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(MinecloneTypes::Vertex), (void *) (3 * sizeof(float)));
    //
    //     // ElementBufferObjects
    //     glGenBuffers(1, &EBO);
    //     glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    //     glBufferData(GL_ELEMENT_ARRAY_BUFFER, meshData.indices.size() * sizeof(unsigned int), meshData.indices.data(),
    //                  GL_STATIC_DRAW);
    // }
}

void World::RenderWorld() const {
    for (const auto &chunk : this->chunks) {
        chunk.second.Draw();
    }
}

void World::GenerationThread() {

}
