#include "banana.h"
#include <QDebug>

Banana::Banana(QObject *parent) : QObject(parent)
{
    this->isThrown = false;

    // set default pixmap
    setPixmap(QPixmap(":/img/res/img_banana_simple.png"));

    // set default direction
    this->direction.setX(0);
    this->direction.setY(-BANANAS_SPEED);

    // connect timer for moving the banana
    moveTimer = new QTimer();
    moveTimer->setInterval(INV_FPS);
    connect(moveTimer, SIGNAL(timeout()), this, SLOT(move()));
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

const QPointF Banana::getCenter()
{
    return QPointF(scenePos().x() + BANANA_SIZE/2, scenePos().y() + BANANA_SIZE/2);
}

bool Banana::thrownStatus() const
{
    return isThrown;
}

void Banana::move()
{
    if (isThrown) {
        setPos(this->scenePos().x() + direction.x(), this->scenePos().y() + direction.y());
//        setTransformOriginPoint(boundingRect().width()/2, boundingRect().height()/2);
        setRotation(rotation() + 10);
    }
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
    isThrown = false;
    moveTimer->stop();
}
