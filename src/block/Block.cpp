//
// Created by Alex on 15/10/2025.
//

#include "Block.h"

Block::Block(const BlockType blockType) {
    this->m_BlockType = blockType;
}

Block::BlockType Block::GetBlockType() const {
    return this->m_BlockType;
}
