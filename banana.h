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
    inline const QPointF getDirection() { return this->direction.toPointF(); }
    inline void setDirection(const float x, const float y) {
        this->direction.setX(x);
        this->direction.setY(y);
    }
    const QPointF getCenter();
    bool thrownStatus() const;

signals:

public slots:
    void move();
    void throwing();
    void crash();

};

#endif // BANANA_H
