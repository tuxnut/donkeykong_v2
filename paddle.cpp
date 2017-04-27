#include "paddle.h"

Paddle::Paddle()
{
    // draw the paddle
    setRect(0, 0, PADDLE_WIDTH, PADDLE_HEGHT);

    // grab input focus
    setFocus();
}

void Paddle::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Right:
        setPos(scenePos().x() + 3, scenePos().y());
        break;
    case Qt::Key_Left:
        setPos(scenePos().x() - 3, scenePos().y());
        break;
    default:
        break;
    }
}

void Paddle::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QPen pen(Qt::gray, 2);
    painter->setPen(pen);
    painter->drawRect(this->rect());
    painter->fillRect(this->rect(), Qt::darkGray);
}
