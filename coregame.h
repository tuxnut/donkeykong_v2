#ifndef COREGAME_H
#define COREGAME_H

#include "model.h"
#include "view.h"
#include "block.h"
#include "player.h"
#include "gamesettings.h"

#include <random>
#include <QTimer>
#include <QMediaPlayer>
#include <QVector2D>

class Model;
class View;

/**
 * @brief The CoreGame class is the controler of our MVC application.
 *  It contains our main loop, deals with passing information to displays to the view, information
 *  harvested from the model. Also the player instance is stored here.
 */
class CoreGame
{
private:
    std::mt19937 rand_gene;
    Model *model;
    View &view;
    Player *dk;
    QMediaPlayer * musicPlayer;
    bool musicOn;

public:
    CoreGame(Model *model, View &view);
    void openGame(const QString &dir);
    int randomGenerator(int min, int max);
    bool changeMusicState();
    void setupGame();
    int updateNbBananas();
    QVector<blockSettings *> setupLevel();
    bool monitorLevel(QList <Banana *> bananas);
    void gameCore(bool loadedGame = false);
    QString getPlayerName();
    void displayHighScores();
    void changeToMenuSound();
    void closeCleanup();
    inline const QVector2D computeAngle(QPointF ban, QPointF bloc, const QVector2D dir) const;
};

const QVector2D CoreGame::computeAngle(QPointF ban, QPointF bloc, const QVector2D dir) const {

    QVector2D res = dir;

    // Working with the center of objects
    ban.rx() += BANANA_SIZE/2;
    ban.ry() += BANANA_SIZE/2;
    bloc.rx() += BLOCK_SIZE/2;
    bloc.rx() += BLOCK_SIZE/2;

    // top / bottom collision
    if ((ban.x() < bloc.x() + BLOCK_SIZE/2 - 5) && (ban.x() > bloc.x() - BLOCK_SIZE/2 + 5) && ((ban.y() > bloc.y() + BLOCK_SIZE/2) || (ban.y() < bloc.y() - BLOCK_SIZE/2))) {
        res.setX(dir.x());
        res.setY(-1 * dir.y());
        return res;
    }
    // right / left collision
    if ((ban.y() < bloc.y() + BLOCK_SIZE/2 - 5) && (ban.y() > bloc.y() - BLOCK_SIZE/2 + 5) && ((ban.x() > bloc.x() + BLOCK_SIZE/2) || (ban.x() < bloc.x() - BLOCK_SIZE/2))) {
        res.setX(-1 * dir.x());
        res.setY(dir.y());
        return res;
    }
    //bottom-left collision
    if ((ban.y() >= bloc.y() + BLOCK_SIZE/2 - 5) && (ban.x() <= bloc.x() - BLOCK_SIZE/2 + 5)) {

        return res;
    }
    //bottom-right collision
    if ((ban.y() >= bloc.y() + BLOCK_SIZE/2 - 5) && (ban.x() >= bloc.x() + BLOCK_SIZE/2 - 5)) {

        return res;
    }
    //top-left collision
    if ((ban.y() <= bloc.y() - BLOCK_SIZE/2 + 5) && (ban.x() <= bloc.x() - BLOCK_SIZE/2 + 5)) {

        return res;
    }
    //top-right collision
    if ((ban.y() <= bloc.y() - BLOCK_SIZE/2 + 5) && (ban.x() >= bloc.x() + BLOCK_SIZE/2 - 5)) {

        return res;
    }
//    return res;
}

#endif // COREGAME_H
