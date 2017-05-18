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
    const QVector2D computeAngle(QPointF ban, QPointF bloc, const QVector2D dir) const;
};
#endif // COREGAME_H
