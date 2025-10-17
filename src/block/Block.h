//
// Created by Alex on 15/10/2025.
//

#ifndef MINECLONE_BLOCK_H
#define MINECLONE_BLOCK_H


class Block {
public:
    enum BlockType {
        AIR,
        DIRT,
        STONE
    };

    Block() : m_BlockType(BlockType::AIR) {}
    explicit Block(BlockType blockType);

    [[nodiscard]] BlockType GetBlockType() const;
private:
    BlockType m_BlockType;
};


#endif //MINECLONE_BLOCK_H