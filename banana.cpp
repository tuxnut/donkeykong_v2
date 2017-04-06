#include "banana.h"

Banana::Banana(QObject *parent) : QObject(parent)
{
    this->isThrown = false;

    // set default pixmap
    setPixmap(QPixmap(":/img/res/img_banana_simple.png"));

    // set default direction
    this->direction.setX(DEFAULT_SPEEDX);
    this->direction.setY(DEFAULT_SPEEDY);

    // connect timer for moving the banana
    moveTimer = new QTimer();
    moveTimer->setInterval(INV_FPS);
    connect(moveTimer, SIGNAL(timeout()), this, SLOT(throwing()));
}

const QPointF Banana::getDirection()
{
    return this->direction.toPointF();
}

void Banana::setDirection(const float x, const float y)
{
    this->direction.setX(x);
    this->direction.setY(y);
}

void Banana::move()
{
    if (isThrown)
        setPos(this->scenePos().x() + direction.x(), this->scenePos().y() + direction.y());
}

void Banana::throwing()
{
    isThrown = true;
    // move the banana to the direction specified by direction
    setPos(this->scenePos().x() + direction.x(), this->scenePos().y() + direction.y());
    moveTimer->start();
}

void Banana::crash()
{
//    isThrown = false;
    moveTimer->stop();
}
