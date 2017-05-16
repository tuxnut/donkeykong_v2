#ifndef BONUSBLOCK_H
#define BONUSBLOCK_H

#include "block.h"
#include "gamesettings.h"

/**
 * @brief The BonusBlock class inherits from the Block class and depicts the behavior of the bonus blocks
 */
class BonusBlock : public Block
{
public:
    BonusBlock(const int bonus);
    void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget);
};

#endif // BONUSBLOCK_H
