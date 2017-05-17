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

    setTransformOriginPoint(getCenter());

    // connect timer for moving the banana
    moveTimer = new QTimer();
    moveTimer->setInterval(INV_FPS);
    connect(moveTimer, SIGNAL(timeout()), this, SLOT(move()));
}

const QPointF Banana::getCenter()
{
    return QPointF(sqrt(2) * (BANANA_SIZE / 2) * qSin(qDegreesToRadians(rotation() + 135)), sqrt(2) * (BANANA_SIZE / 2) * qSin(qDegreesToRadians(rotation() + 45)));
}

bool Banana::thrownStatus() const
{
    return isThrown;
}

void Banana::move()
{
//    QGraphicsRectItem * rect = new QGraphicsRectItem(this);
//    QGraphicsRectItem * rect2 = new QGraphicsRectItem(this);
//    rect->setRect(this->boundingRect());
//    rect2->setRect(getCenter().x(),getCenter().y(),1,1);
    if (isThrown) {
        setPos(scenePos() + getDirection());

//        setTransformOriginPoint(getCenter());
//        setRotation(rotation() + 3);
    }

//    qDebug()<< ;
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
