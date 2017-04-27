#ifndef PADDLE_H
#define PADDLE_H

#include <QGraphicsRectItem>
#include <QPen>
#include <QPainter>
#include <QKeyEvent>
#include "gamesettings.h"

class Paddle : public QGraphicsRectItem
{
public:
    Paddle();
    void keyPressEvent(QKeyEvent * event);
    void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget);
};

#endif // PADDLE_H
