#ifndef PLAYER_H
#define PLAYER_H

#include <QGraphicsPixmapItem>
#include <QObject> // need this for the connect signal / slot
#include <QMovie>
#include <QKeyEvent>

#define BANANAS_DEFAULT 2;

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
//    Player(); // TO DEFINE when loading of an previous game
    int getNbBananas() const;
    int getScore() const;
    int getNbBlockDestroyed() const;
    void setNbBananas(const int nbBananas = 0);
    void setScore(const int score = 0);
    void setNbBlockDestroyed(const int nbBlocks = 0);
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
