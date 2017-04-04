#include "coregame.h"
#include <QDebug>

CoreGame::CoreGame(Model *model, View &view) :
    view(view)
{
    this->model = model;
    view.setControl(this);
}

int CoreGame::randomGenerator(int min, int max)
{
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_real_distribution<double> dist(min, std::nextafter(max, INT_MAX));
    return qAbs(dist(mt));
}

void CoreGame::setupGame()
{
    Player * dk = new Player();

    if (!dk)
        return;

    view.displayGame(dk);
    gameCore();
}

int CoreGame::updateNbBananas()
{
    // update nb of pixmap bananas to put into the scene
    if (view.getPlayer()->getNbBananas() != view.getNbPixmapBanana())
        return view.getPlayer()->getNbBananas() - view.getNbPixmapBanana();
    else
        return 0;
}

int * CoreGame::setupLevel()
{    
    // nb of blocks we will create on a level (0 to max block on a single block line)
    int nbBlockToCreate = randomGenerator(0, MAX_BLOCKLINE);

    if (nbBlockToCreate > 0) {
        QList <int> posXTable;
        for (int i = 0; i < MAX_BLOCKLINE; i++) {
            posXTable.append(i);
        }
        // each block will have two settings : posX (we already know posY) and the number of bananas to
        // destroy it
        int * blockSettings = (int*) malloc((nbBlockToCreate * 2 + 1) * sizeof(int));
        blockSettings[0] = nbBlockToCreate;

        for (int i = 1; i < nbBlockToCreate * 2; i += 2) {
            // set the position of the block (as the posx is set randomly, we don't want two blocks at the same position)
            int posX = randomGenerator(0, posXTable.size());
            blockSettings[i] = posX * BLOCK_SIZE;
            posXTable.removeAt(posX);

            // set the points that the block will have
            blockSettings[i+1] = randomGenerator(1, view.getPlayer()->getNbBananas());
        }
        return blockSettings;

    } else
        return NULL;

}

void CoreGame::gameCore()
{
    view.displayLevel();
    view.playerAction();
}


