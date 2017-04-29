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
    model->setControl(this);
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

    if (model->loadPlayer(dk, dir)) {
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

/**
 * @brief CoreGame::setupLevel : informs the view about how many blocks should be created for the level with their caracteristic (nb of points, bonu type, ...)
 * @return QVector <blockSettings*> : the blockSettings structure contains all the data necessary to create a block : nb of points, pos-x, bonus type
 */
QVector<blockSettings*> CoreGame::setupLevel()
{
    QVector<blockSettings*> blockSetting;

    // nb of blocks we will create on a level (1 to max block on a single block line)
    int nbBlockToCreate = randomGenerator(1, MAX_BLOCKLINE);

    if (nbBlockToCreate > 0) {
        QList<int> posXTable;
        for (int i = 0; i < MAX_BLOCKLINE; i++) {
            posXTable.append(i);
        }
        // each block will have two settings : posX (we already know posY) and the number of bananas to
        // destroy it
        for (int i = 0; i < nbBlockToCreate; i++) {
            blockSettings * bS = new blockSettings();

            // set the position of the block (as the posx is set randomly, we don't want two blocks at the same position)
            int indice = randomGenerator(0, posXTable.size());
            int posX = posXTable[indice];
            bS->posX = posX * BLOCK_SIZE;
            posXTable.removeAt(indice);

            // chances the block may become a bonus block - a bonus block has no point
            int bonus = randomGenerator(0, 100);
            if (bonus > 85) {
                bS->posX += BLOCK_SIZE/4;
                bS->bonusType = MORE_BANANA_BONUS;
                bS->point = 0;
            } else if (bonus > 95) {
                bS->bonusType = PADDLE_BONUS;
                bS->point = 0;
            } else if (bonus > 97) {
                bS->bonusType = MORE_LIFE_BANANA_BONUS;
                bS->point = 0;
            } else {
                // set the number of point the new block will have :
                bS->bonusType = 0;
                bS->point = randomGenerator(1, dk->getNbBananas());
            }

            blockSetting.append(bS);
        }
    }
    return blockSetting;
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
    // If the level is at least the second one
    if (dk->getScore() > 0 && (!loadedGame)) {
        // check ig perfect level -> our condition to 'autosave' the game, and make a checkpoint
        if (view.checkPerfectLevel()) {
            model->saveGameAuto(dk);
            dk->setLastCheckpoint(dk->getScore());
        }
    }
    // update nb banana and display the new line of blocks
    view.displayLevel();

    // reposition the player and its bananas
    view.repositionPlayer();

    // basivally our condition whether it's game over or not
    if (view.lowerBlocks()) {
        // game continues
        dk->setScore();
        view.incScoreBoard();
        view.playerAction();
    } else {
        // game is over
        view.gameOver();
        // check if score is highscore AND writes it on the highscore.dat file
        model->isHighScore(dk->getScore());

        view.cleanUpGame();
        if (view.playerLoadCheckpoint()) {
            model->loadPlayer(dk);
            gameCore(true);
        } else {
            view.closeEvent(new QCloseEvent());
        }
    }
}

/**
 * @brief CoreGame::getPlayerName : returns the name of the player (make the link between model and view)
 * @return QString
 */
QString CoreGame::getPlayerName()
{
    const QString &str = view.getPlayerName();
    return str;
}

/**
 * @brief CoreGame::displayHighScores : search for the highscores towards the model and gives them to the view
 */
void CoreGame::displayHighScores()
{
    QVector<Qhighscore> vectorQhighscore = model->getHighScores();
    if (vectorQhighscore.isEmpty())
        qWarning("couldn't parse the highscore file");
    else
        view.displayHighScores(vectorQhighscore);
}

/**
 * @brief CoreGame::closeCleanup : should clean up the memory before closing the app
 */
void CoreGame::closeCleanup()
{
//    free(dk);
//    delete dk;
    dk = nullptr;
}
