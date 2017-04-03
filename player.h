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
    int blockDestroyed;

public:
    explicit Player(QObject *parent = 0);
//    Player(); // TO DEFINE for the loading of an previous game
    int getNbBananas();
    inline int getScore();
    inline int getNbBlockDestroyed();
    inline void setNbBananas(int nbBananas);
    inline void setScore(int score);
    inline void setNbBlockDestroyed(int nbBlocks);
    void animation(QMovie * source);

signals:

public slots:
    void changeFrame();
};

#endif // PLAYER_H
