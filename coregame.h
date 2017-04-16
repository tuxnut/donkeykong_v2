#ifndef COREGAME_H
#define COREGAME_H

#include "model.h"
#include "view.h"
#include "block.h"
#include "player.h"
#include "visitor.h"
#include <random>
#include <QTimer>

#define MAX_BLOCKLINE 12

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
    int * setupLevel();
    bool monitorGame(QList <Banana *> bananas);
    void gameCore(bool loadedGame = false);
};

#endif // COREGAME_H
