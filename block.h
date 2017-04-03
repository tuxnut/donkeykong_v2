#ifndef BLOCK_H
#define BLOCK_H

#include <QGraphicsRectItem>

#define BLOCK_SIZE 60

class Block : public QObject, public QGraphicsRectItem
{
    Q_OBJECT
public:
    explicit Block(QObject *parent = 0);

signals:

public slots:
};

#endif // BLOCK_H
