#include "coregame.h"

CoreGame::CoreGame(Model *model, View &view) :
    view(view)
{
    this->model = model;
    view.setControl(this);
}

void CoreGame::setupGame()
{
    dk = new Player();

    if (!dk)
        return;

    view.displayGame(dk);
}
