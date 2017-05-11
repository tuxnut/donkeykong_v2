#ifndef PLAYER_H
#define PLAYER_H

#include "gamesettings.h"

#include <QGraphicsPixmapItem>
#include <QObject> // need this for the connect signal / slot
#include <QMovie>
#include <QKeyEvent>

/**
 * @class The Player class depicts the behavior of the player, and contains its number of bananas, score and more.
 */
class Player : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT

private:
    QMovie *source;
    int bananas;
    int score;
    int blockDestroyed;
    int lastCheckpoint;

public:
    explicit Player(QObject *parent = 0);
//    Player(); // TO DEFINE when loading of an previous game
    int getNbBananas() const;
    int getScore() const;
    int getNbBlockDestroyed() const;
    int getLastCheckpoint() const;
    void setNbBananas(const int nbBananas = 0);
    void setScore(const int score = 0);
    void setNbBlockDestroyed(const int nbBlocks = 0);
    void setLastCheckpoint(const int checkpoint);
    void animation(QMovie * source);
    void keyPressEvent(QKeyEvent * event);

signals:
    void leanRight();
    void leanLeft();
    void launch();

public slots:
    void changeFrame();
};

#endif // PLAYER_H
