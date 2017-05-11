#ifndef BLOCK_H
#define BLOCK_H

#include "gamesettings.h"

#include <QGraphicsRectItem>
#include <QPen>
#include <QPainter>

/**
 * @class The Block class depicts the behavior of the blocks
 */
class Block : public QObject, public QGraphicsRectItem
{
    Q_OBJECT
private:
    QGraphicsSimpleTextItem * textItem;
    int points;
    int bonus;

public:
    explicit Block(QObject *parent = 0);
    Block(int points);
    bool decPoints();
    int getBonusType() const;
    void setBonus(int type);
    void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget);

signals:

public slots:
};

#endif // BLOCK_H
