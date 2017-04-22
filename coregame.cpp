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
 * @brief CoreGame::openGame :open a saved game, asking the model to open the file
 * @param dir : directory of the saved game file
 */
void CoreGame::openGame(const QString &dir)
{
    dk = new Player();
    if (!dk)
        qWarning("Error creating player");

    if (model->loadPlayer(dir, dk)) {
        view.displayGame(dk);
        gameCore(true);
    } else
        qWarning("Error loading saved game file");
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

/**
 * @brief CoreGame::updateNbBananas : update nb of pixmap bananas to put into the scene
 * @return
 */
int CoreGame::updateNbBananas()
{
    return qAbs(dk->getNbBananas() - view.getNbPixmapBanana());
}

QVector<int> CoreGame::setupLevel()
{
    QVector<int> blockSettings;

    // nb of blocks we will create on a level (1 to max block on a single block line)
    int nbBlockToCreate = randomGenerator(1, MAX_BLOCKLINE);

    if (nbBlockToCreate > 0) {
        QList<int> posXTable;
        for (int i = 0; i < MAX_BLOCKLINE; i++) {
            posXTable.append(i);
        }
        // each block will have two settings : posX (we already know posY) and the number of bananas to
        // destroy it
        for (int i = 0; i < nbBlockToCreate * 2; i += 2) {
            // set the position of the block (as the posx is set randomly, we don't want two blocks at the same position)
            int indice = randomGenerator(0, posXTable.size());
            int posX = posXTable[indice];
            blockSettings.append(posX * BLOCK_SIZE);
            posXTable.removeAt(indice);

            // chances the block may become a bonus block - a bonus block has no point
            if (randomGenerator(0, 100) > 85) {
                blockSettings[i] += BLOCK_SIZE/4;
                blockSettings.append(0);
            } else {
                // set the points that the block will have
                blockSettings.append(randomGenerator(1, dk->getNbBananas()));
            }

        }
    }
    return blockSettings;
}

/**
 * @brief CoreGame::monitorLevel : checks if every bananas have hit the floor which would mean the end of the level
 * @param bananas : QList<Banana *>
 * @return : true if every bananas have hit the floor, false otherwise (level still incomplete)
 */
bool CoreGame::monitorLevel(QList<Banana *> bananas)
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
void CoreGame::gameCore(bool loadedGame)
{
    if (dk->getScore() > 0 && (!loadedGame)) {
        if (view.checkPerfectLevel()) {
            model->saveGameAuto(dk);
        }
    }
    view.displayLevel();
    view.repositionPlayer();
    if (view.lowerBlocks()) {
        dk->setScore();
        view.incScoreBoard();
        view.playerAction();
    } else {
        /* game lost */
    }
}


