#ifndef BANANA_H
#define BANANA_H

#include <QObject>
#include <QGraphicsPixmapItem>

class Banana : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    explicit Banana(QObject *parent = 0);

signals:

public slots:
};

#endif // BANANA_H
