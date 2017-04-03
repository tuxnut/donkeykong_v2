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
    view.displayLevel();
//    gameCore(); // TODO !!!
}

QList<int *> CoreGame::setupLevel()
{
    QList <int *> listBlock;

    // nb of blocks we will create on a level (0 to max block on a single block line)
    int nbBlockToCreate = qrand() % (VIEW_WIDTH / BLOCK_SIZE);

    // creation of blocks
    for (int i = 0; i < nbBlockToCreate; i++) {
        // each block will have two settings : posX (we already know posY) and the number of bananas to
        // destroy it
        int *blockSettings = (int*) malloc(2 * sizeof(int));
        int posX = (qrand() % (VIEW_WIDTH / BLOCK_SIZE)) * BLOCK_SIZE;
        int points = qrand() % this->dk->getNbBananas();

        listBlock.append(blockSettings);

        // as the posx is set randomly, we don't want two blocks at the same position :
        for (int j = 0; j < i; j++) {
            if (posX == listBlock[j][0])
                posX = (qrand() % (VIEW_WIDTH / BLOCK_SIZE)) * BLOCK_SIZE;
        }

        blockSettings[0] = posX;
        blockSettings[1] = points;
    }

    return listBlock;
}


