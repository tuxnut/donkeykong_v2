#include "coregame.h"
#include <QDebug>

/**
 * @brief CoreGame::CoreGame : basic constructor, links the controler to the view and the model
 * @param model Model of the app
 * @param view View of the app
 */
CoreGame::CoreGame(Model *model, View &view) :
    view(view)
{
    this->model = model;
    view.setControl(this);
}

/**
 * @brief CoreGame::randomGenerator : generates random integer numbers between min and max (included)
 * @param min : minimum parameter
 * @param max : maximum parameter
 * @return
 */
int CoreGame::randomGenerator(int min, int max)
{
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_real_distribution<double> dist(min, std::nextafter(max, INT_MAX));
    return qAbs(dist(mt));
}

/**
 * @brief CoreGame::setupGame : launches a new game, with a new Player and starts the 'loop'
 */
void CoreGame::setupGame()
{
    dk = new Player();

    if (!dk)
        return;

    view.displayGame(dk);
    gameCore();
}

int CoreGame::updateNbBananas()
{
    // update nb of pixmap bananas to put into the scene
    if (dk->getNbBananas() != view.getNbPixmapBanana())
        return dk->getNbBananas() - view.getNbPixmapBanana();
    else
        return 0;
}

int * CoreGame::setupLevel()
{    
    // nb of blocks we will create on a level (1 to max block on a single block line)
    int nbBlockToCreate = randomGenerator(1, MAX_BLOCKLINE);

    if (nbBlockToCreate > 0) {
        QList <int> posXTable;
        for (int i = 0; i < MAX_BLOCKLINE; i++) {
            posXTable.append(i);
        }
        // each block will have two settings : posX (we already know posY) and the number of bananas to
        // destroy it
        int * blockSettings = (int*) malloc((nbBlockToCreate * 2 + 1) * sizeof(int));
        blockSettings[0] = nbBlockToCreate; // ~size

        for (int i = 1; i < nbBlockToCreate * 2; i += 2) {
            // set the position of the block (as the posx is set randomly, we don't want two blocks at the same position)
            int indice = randomGenerator(0, posXTable.size());
            int posX = posXTable[indice];
            blockSettings[i] = posX * BLOCK_SIZE;
            posXTable.removeAt(indice);

            // set the points that the block will have
            blockSettings[i+1] = randomGenerator(1, dk->getNbBananas());
        }
        return blockSettings;

    } else
        return NULL;

}

bool CoreGame::monitorGame(QList<Banana *> bananas)
{
    foreach (Banana * ban, bananas) {
        if (ban->thrownStatus())
            return false;
    }
    return true;
}

/**
 * @brief CoreGame::gameCore : game loop
 */
void CoreGame::gameCore()
{
    if (dk->getScore() > 0) {
        if (view.checkPerfectLevel()) {
            /* visitor codes saving game here */
        }
        view.repositionPlayer();
    }
    view.displayLevel();
    if (view.lowerBlocks()) {
        dk->setScore();
        view.playerAction();
    } else {
        /* game lost */
    }
}


