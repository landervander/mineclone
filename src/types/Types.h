//
// Created by Alex on 18/10/2025.
//

#ifndef MINECLONE_TYPES_H
#define MINECLONE_TYPES_H

#include <vector>

namespace MinecloneTypes {
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

    struct MeshData {
        int chunkX, chunkZ;
        std::vector<MinecloneTypes::Vertex> vertices;
        std::vector<unsigned int> indices;
    };
}


#endif //MINECLONE_TYPES_H