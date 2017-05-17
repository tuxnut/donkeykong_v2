#ifndef BANANA_H
#define BANANA_H

#include "gamesettings.h"
#include "unistd.h"
#include <QObject>
#include <QGraphicsPixmapItem>
#include <QVector2D>
#include <QTimer>
#include <QtMath>

/**
 * @brief The Banana class depicts the projectile the player thows
 * Basically, they move thanks to a Qtimer.
 */
class Banana : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT

private:
    bool isThrown;
    QTimer * moveTimer;
    QVector2D direction;

public:
    explicit Banana(QObject *parent = 0);
    inline const QPointF getDirectionToPointF() { return this->direction.toPointF(); }
    inline const QVector2D getDirection() { return this->direction; }
    inline void setDirection(const QVector2D newDirection) {
        this->direction = newDirection;
    }
    inline void setDirection(const float x, const float y) {
        this->direction.setX(x);
        this->direction.setY(y);
    }
//    inline const QPointF getCenter() {
//        return QPointF (scenePos.x() + BANANA_SIZE/2, scenePos.y() + BANANA_SIZE/2);
//        QPointF(sqrt(2) * (BANANA_SIZE / 2) * qSin(qDegreesToRadians(rotation() + 135)), sqrt(2) * (BANANA_SIZE / 2) * qSin(qDegreesToRadians(rotation() + 45)));
//    }

    bool thrownStatus() const;

signals:

public slots:
    void move();
    void throwing();
    void crash();

};

#endif // BANANA_H
