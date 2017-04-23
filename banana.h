#ifndef BANANA_H
#define BANANA_H

#include "gamesettings.h"
#include "unistd.h"
#include <QObject>
#include <QGraphicsPixmapItem>
#include <QVector2D>
#include <QTimer>

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
