#ifndef COREGAME_H
#define COREGAME_H

#include "model.h"
#include "view.h"
#include "block.h"
#include "player.h"
#include "visitor.h"
#include "gamesettings.h"

#include <random>
#include <QTimer>

class Model;
class View;

class CoreGame
{
private:
    std::mt19937 rand_gene;
    Model *model;
    View &view;
    Player *dk;

public:
    CoreGame(Model *model, View &view);
    void openGame(const QString &dir);
    int randomGenerator(int min, int max);
    void setupGame();
    int updateNbBananas();
    QVector <int> setupLevel();
    bool monitorLevel(QList <Banana *> bananas);
    void gameCore(bool loadedGame = false);
    QString getPlayerName();
    void displayHighScores();
    void closeCleanup();
};

#endif // COREGAME_H
