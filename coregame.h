#ifndef COREGAME_H
#define COREGAME_H

#include "model.h"
#include "view.h"
#include "block.h"
#include "player.h"

class Model;
class View;

class CoreGame
{
private:
    Model *model;
    View &view;
    Player *dk;

public:
    CoreGame(Model *model, View &view);
    void setupGame();
    QList<int *> setupLevel();
};

#endif // COREGAME_H
