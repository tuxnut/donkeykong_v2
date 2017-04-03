#ifndef BLOCK_H
#define BLOCK_H

#include <QGraphicsRectItem>

#define BLOCK_SIZE 50

class Block : public QObject, public QGraphicsRectItem
{
    Q_OBJECT
private:
    int points;

public:
    explicit Block(QObject *parent = 0);
    Block(int points);
    inline int getPoints();

signals:

public slots:
};

#endif // BLOCK_H
