#include "block.h"

Block::Block(QObject *parent) : QObject(parent)
{
    // draw the block
    setRect(0,0, BLOCK_SIZE, BLOCK_SIZE);
}

Block::Block(int points)
{
    // draw the block
    setRect(0,0, BLOCK_SIZE, BLOCK_SIZE);
    this->points = points;
}

int Block::getPoints()
{
    return this->points;
}
