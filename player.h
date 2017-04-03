#ifndef PLAYER_H
#define PLAYER_H

#include <QGraphicsPixmapItem>
#include <QObject> // need this for the connect signal / slot
#include <QMovie>

class Player : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
private:
    QMovie *source;
    int bananas;
    int score;

public:
    explicit Player(QObject *parent = 0);
    inline int getNbBananas();
    inline int getScore();
    inline void setNbBananas(int nbBananas);
    inline void setScore(int score);
    void animation(QMovie * source);

signals:

public slots:
    void changeFrame();
};

#endif // PLAYER_H
