#ifndef PADDLE_H
#define PADDLE_H

#include <QGraphicsRectItem>
#include <QPen>
#include <QPainter>
#include <QKeyEvent>
#include "gamesettings.h"

/**
 * @brief The Paddle class depicts the behavior of the paddle within the game
 */
class Paddle : public QGraphicsRectItem
{
public:
    Paddle();
    void keyPressEvent(QKeyEvent * event);
    void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget);
};

#endif // PADDLE_H
