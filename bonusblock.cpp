#include "bonusblock.h"

BonusBlock::BonusBlock(const int bonus)
{
    // draw the block
    setRect(0,0, BLOCK_SIZE/2, BLOCK_SIZE/2);
    this->points = 0;

    // default bonus
    this->bonus = bonus;
}

void BonusBlock::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QPen pen(Qt::red, 2);
    switch (bonus) {
    case MORE_BANANA_BONUS:
        {
        painter->setPen(pen);
        painter->drawEllipse(this->rect());
        break;
        }
    case PADDLE_BONUS:
        {
        pen.setBrush(Qt::blue);
        painter->setPen(pen);
        painter->drawEllipse(this->rect());
        break;
        }
    case MORE_LIFE_BANANA_BONUS:
        {
        pen.setBrush(Qt::green);
        painter->setPen(pen);
        painter->drawEllipse(this->rect());
        break;
        }
    default:
        {
        pen.setBrush(Qt::black);
        painter->setPen(pen);
        painter->drawEllipse(this->rect());
        break;
        }
    }
}
