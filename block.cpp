#include "block.h"

#include <QDebug>

Block::Block(QObject *parent) : QObject(parent)
{
    // draw the block
    setRect(0,0, BLOCK_SIZE, BLOCK_SIZE);

    bonus = 0;
}

Block::Block(int points)
{
    // draw the block
    setRect(0,0, BLOCK_SIZE, BLOCK_SIZE);
    this->points = points;

    // default bonus
    bonus = 0;

    // draw the number of points inside
    textItem = new QGraphicsSimpleTextItem(QString::number(this->points), this);
}

bool Block::decPoints()
{
    points--;
    textItem->setText(QString::number(this->points));
    if (points == 0)
        return true;
    else
        return false;
}

int Block::getBonusType() const
{
    return bonus;
}

/**
 * @brief Block::setBonus : set a bonus for the block instead a a classic block
 * @param type : set the type of bonus for the block
 */
void Block::setBonus(int type)
{
    points = 0;
    textItem->setText("");
    setRect(0, 0, BLOCK_SIZE/2, BLOCK_SIZE/2);
    bonus = type;
}

void Block::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    if (bonus == MORE_BANANA_BONUS) {
        QPen pen(Qt::red, 2);
        painter->setPen(pen);
        painter->drawEllipse(this->rect());
    } else if (bonus == PADDLE_BONUS){
        QPen pen(Qt::blue, 2);
        painter->setPen(pen);
        painter->drawEllipse(this->rect());
    } else if (bonus == MORE_LIFE_BANANA_BONUS){
        QPen pen(Qt::green, 2);
        painter->setPen(pen);
        painter->drawEllipse(this->rect());
    } else if (0 < points && points < 10) {
        QPen pen(Qt::red, 1);
        painter->setPen(pen);
        painter->drawRect(this->rect());
        painter->fillRect(this->rect(), Qt::yellow);
    } else if (10 <= points && points < 20 ) {
        QPen pen(Qt::blue, 1);
        painter->setPen(pen);
        painter->drawRect(this->rect());
        painter->fillRect(this->rect(), Qt::darkCyan);
    }
}
