#ifndef COREGAME_H
#define COREGAME_H

#include "model.h"
#include "view.h"
#include "block.h"
#include "player.h"
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
    int randomGenerator(int min, int max);
    void setupGame();
    int updateNbBananas();
    int * setupLevel();
    bool monitorGame(QList <Banana *> bananas);
    void gameCore();
};

#endif // COREGAME_H
