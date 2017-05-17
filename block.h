#ifndef BLOCK_H
#define BLOCK_H

#include "gamesettings.h"

#include <QGraphicsRectItem>
#include <QPen>
#include <QPainter>

/**
 * @brief The Block class depicts the behavior of the blocks
 */
class Block : public QObject, public QGraphicsRectItem
{
    Q_OBJECT
protected:
    QGraphicsSimpleTextItem * textItem;
    int points;
    int bonus;

public:
    explicit Block(QObject *parent = 0);
    Block(int points);
    inline bool decPoints() {
        points--;
        textItem->setText(QString::number(this->points));
        if (points == 0)
            return true;
        else
            return false;
    }
    inline int getBonusType() const { return bonus; }
    void setBonus(int type);
    void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget);

signals:

public slots:
};

#endif // BLOCK_H
