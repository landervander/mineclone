//
// Created by Alex on 17/10/2025.
//

#include "World.h"

#include <thread>

#include "imgui.h"

World::World() {
    printf("World thread started\n");
    std::thread worldGen(&World::GenerationThread, this);
    worldGen.detach();
    running = true;
}

void World::GenerateWorld() {
    // Generate chunk data, don't build meshes
    for (int x = -viewDistance; x < viewDistance; x++) {
        for (int y = -viewDistance; y < viewDistance; y++) {
            Chunk *chunk = new Chunk();
            chunk->SetChunkPosition(x, y);
            chunk->GenerateBlocks();

            ChunkMesh* chunkMesh = new ChunkMesh(*chunk);
            chunkMesh->BuildMesh();

            this->chunks.emplace_back(chunk);
            this->chunksToRender.emplace_back(chunkMesh);
        }
    }
}

void World::Update() {
    if (!this->chunkMeshQueue.empty()) {
        const auto& chunkMesh = this->chunkMeshQueue.front();

        // VertexArrayObject
        glGenVertexArrays(1, &chunkMesh->VAO);
        glBindVertexArray(chunkMesh->VAO);

        // VertexBufferObject
        glGenBuffers(1, &chunkMesh->VBO);
        glBindBuffer(GL_ARRAY_BUFFER, chunkMesh->VBO);
        glBufferData(GL_ARRAY_BUFFER, chunkMesh->cubeVertices.size() * sizeof(MinecloneTypes::Vertex), chunkMesh->cubeVertices.data(), GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(MinecloneTypes::Vertex), NULL);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(MinecloneTypes::Vertex), (void *) (3 * sizeof(float)));

        // ElementBufferObjects
        glGenBuffers(1, &chunkMesh->EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, chunkMesh->EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, chunkMesh->cubeIndices.size() * sizeof(unsigned int), chunkMesh->cubeIndices.data(),
                     GL_STATIC_DRAW);

        this->chunkMeshQueue.pop();
        this->chunksToRender.emplace_back(chunkMesh);
    }
}

void World::RenderUI() {
    // TODO: PLEAAAAAAAAASE REFACTOR THIS
    ImGui::SetNextWindowPos(ImVec2(15, 15 + 150 + 15));
    ImGui::SetNextWindowSize(ImVec2(150, 150));
    ImGui::Begin("World", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoDecoration);
    ImGui::Text("FPS: %.2f", ImGui::GetIO().Framerate);
    ImGui::Text("Total Chunks: %d", this->chunksToRender.size());
    ImGui::End();
}

void World::RenderWorld() {
    // Disable rendering for a sec
    for (const auto &chunkMesh : this->chunksToRender) {
        glBindVertexArray(chunkMesh->VAO);
        glDrawElements(GL_TRIANGLES, chunkMesh->cubeIndices.size(), GL_UNSIGNED_INT, NULL);
    }
}

Chunk *World::GetChunkAt(int x, int y) const {
    for (const auto &chunk: this->chunks) {
        if (chunk->chunkPos == glm::ivec2(x, y)) {
            return chunk;
        }
    }
    return nullptr;
}

std::vector<glm::ivec2> GenerateSpiralOrder(int radius, int centerX, int centerY) {
    std::vector<glm::ivec2> order;
    int x = 0, y = 0;
    int dx = 0, dy = -1;
    int max = (radius * 2 + 1) * (radius * 2 + 1);

    for (int i = 0; i < max; i++) {
        if (abs(x) <= radius && abs(y) <= radius)
            order.emplace_back(centerX + x, centerY + y);

        if (x == y || (x < 0 && x == -y) || (x > 0 && x == 1 - y)) {
            int temp = dx;
            dx = -dy;
            dy = temp;
        }

        x += dx;
        y += dy;
    }

    return order;
}

void World::GenerationThread() {
    while (running) {

        auto positions = GenerateSpiralOrder(viewDistance, 0.f, 0.f);
        for (const auto &pos: positions) {
            if (this->GetChunkAt(pos.x, pos.y) != nullptr) continue;

            Chunk *chunk = new Chunk();
            chunk->SetChunkPosition(pos.x, pos.y);
            chunk->GenerateBlocks();

            ChunkMesh* chunkMesh = new ChunkMesh(*chunk);
            chunkMesh->BuildMesh();

            this->chunks.emplace_back(chunk);
            this->chunkMeshQueue.emplace(chunkMesh);
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}
