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

    // draw the number of points inside
    textItem = new QGraphicsSimpleTextItem(QString::number(this->points), this);
}

bool Block::decPoints()
{
    points--;
    if (points == 0)
        return true;
    else
        return false;
}
