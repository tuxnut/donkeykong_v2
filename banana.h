#ifndef BANANA_H
#define BANANA_H


#include "unistd.h"
#include <QObject>
#include <QGraphicsPixmapItem>
#include <QVector2D>
#include <QTimer>

// number of px -> 60fps (0.01667s)
#define DEFAULT_SPEEDX 0
#define DEFAULT_SPEEDY -4
#define BANANA_SIZE 20
#define INV_FPS 17      // 1/fps = 1/60 ~= 17ms

class Banana : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT

private:
    bool isThrown;
    QTimer * moveTimer;
    QVector2D direction;

public:
    explicit Banana(QObject *parent = 0);
    const QPointF getDirection();
    void setDirection(const float x, const float y);
    const QPointF getCenter();
    bool thrownStatus() const;

signals:

public slots:
    void move();
    void throwing();
    void crash();

};

#endif // BANANA_H
