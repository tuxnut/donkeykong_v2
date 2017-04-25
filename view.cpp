#include "view.h"
#include "ui_view.h"
#include "ui_highscore.h"
#include "coregame.h"

#include <QDebug>
#include <QElapsedTimer>

/**
 * @brief View::View : constructor, setting things up here (the QGraphicsScene and some QTimers)
 * @param parent
 */
View::View(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::View)
{
    ui->setupUi(this);
    setFixedSize(VIEW_WIDTH, VIEW_HEIGHT);
    scene = new QGraphicsScene(this);
    blocks = new QGraphicsItemGroup();
    scene->addItem(blocks);

    // this will allows to detect collision
    refreshTimer = new QTimer();
    refreshTimer->setInterval(REFRESH_COLLISION);
    connect(this->refreshTimer, SIGNAL(timeout()), this, SLOT(collision()));

    // this will ask the controler whether or not the bananas have all crashed
    monitoringTimer = new QTimer();
    monitoringTimer->setInterval(MONITORING_INTERVAL);
    connect(this->monitoringTimer, SIGNAL(timeout()), this, SLOT(monitorLevel()));
}

View::~View()
{
    delete ui;
}

/**
 * @brief View::getPlayer : getter for the player. May be useless since the controler knows it too ...
 * @return  : current instance of player
 */
Player *View::getPlayer()
{
    return this->dk;
}

/**
 * @brief View::setControl : so the controler and the view get to know each other
 * @param control
 */
void View::setControl(CoreGame *control)
{
    this->control = control;
}

/**
 * @brief View::getNbPixmapBanana : getter for the size of the QList<Banana *> that are actually QGraphicsPixmapItems
 * @return  : return the size of the list
 */
int View::getNbPixmapBanana()
{
    return this->bananas.size();
}

/**
 * @brief View::displayGame : setups the ingredients for a new game and place the elements in the view (QGraphicsView, QGraphicsLines, Player, ...)
 * @param dk : the new player instance
 */
void View::displayGame(Player *dk)
{
    this->dk = dk;

    // setting up the QgraphicsView / Scene
    QGraphicsView * gameView = new QGraphicsView(scene, this);
    gameView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    gameView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    gameView->setFixedSize(VIEW_WIDTH, VIEW_HEIGHT);
    scene->setSceneRect(0, 0, VIEW_WIDTH, VIEW_HEIGHT);
    gameView->setBackgroundBrush(QBrush(QPixmap(":/img/res/bck_img_game.png")));

    QGraphicsLineItem * topLine = new QGraphicsLineItem(0, TOP_LINE_HEIGHT, VIEW_WIDTH, TOP_LINE_HEIGHT);
    QGraphicsLineItem * bottomLine = new QGraphicsLineItem(0, BOTTOM_LINE_HEIGHT, VIEW_WIDTH, BOTTOM_LINE_HEIGHT);
    scene->addItem(topLine);
    scene->addItem(bottomLine);

    scoreItem = new QGraphicsTextItem("Score : " + QString::number(dk->getScore()));
//    scoreItem->setFont();
//    scoreItem->setDefaultTextColor();
//    scoreItem->setPos();
    blockDestrItem = new QGraphicsTextItem("Nombre de Blocks Détruits : " + QString::number(dk->getNbBlockDestroyed()));
//    scoreItem->setFont();
//    scoreItem->setDefaultTextColor();
    blockDestrItem->setPos(BLOCK_DESTR_POSX, 0);
    bananaItem = new QGraphicsTextItem("Bananes : " + QString::number(dk->getNbBananas()));
//    scoreItem->setFont();
//    scoreItem->setDefaultTextColor();
    bananaItem->setPos(0, BOTTOM_LINE_HEIGHT + 5);
    scene->addItem(scoreItem);
    scene->addItem(blockDestrItem);
    scene->addItem(bananaItem);

    // adding the player
    dk->setPos(PLAYER_POSX, PLAYER_POSY);
    dk->setFlag(QGraphicsItem::ItemIsFocusable);
    dk->setFocus();
    scene->addItem(dk);

    // connect the signals to the view
    connect(dk, SIGNAL(leanRight()), this, SLOT(playerAxisLeanRight()));
    connect(dk, SIGNAL(leanLeft()), this, SLOT(playerAxisLeanLeft()));
    connect(dk, SIGNAL(launch()), this, SLOT(startPlaying()));

    playerAxis = new QGraphicsLineItem();
    playerAxis->setVisible(false);
    scene->addItem(playerAxis);
    gameView->show();
}

/**
 * @brief View::displayLevel : interacts with the controler to display a new level : the ancient block not yet destroyed go down a bit and some are created
 */
void View::displayLevel()
{
    // update the number of pixmap of bananas
    int bananaToCreate = this->control->updateNbBananas();
    for (int i = 0; i < bananaToCreate; i++) {
        bananas.append(new Banana());
        bananas.last()->setPos(PLAYER_POSX + PLAYER_SIZE/4, PLAYER_POSY);
        scene->addItem(bananas.last());
    }

    // ask the controler what to add to the view
    QVector<int> blockSettings = this->control->setupLevel();

    if (blockSettings.isEmpty())
        return;

    for (int i = 0; i < blockSettings.size(); i += 2) {
        Block * block = new Block(blockSettings[i+1]);
        if (blockSettings[i+1] == 0) {
            block->setBonus(1);
            block->setPos(blockSettings[i], TOP_LINE_HEIGHT + BLOCK_SIZE/4);
        } else {
            block->setPos(blockSettings[i], TOP_LINE_HEIGHT);
        }

        blocks->addToGroup(block);
    }
}

/**
 * @brief View::playerAction : let the player choose a direction to send the bananas. We change to an asynchronous behavior from here. It's also the player that handles the keyboard inputs.
 */
void View::playerAction()
{
    scene->clearFocus();
    scene->setFocusItem(dk);
    playerAxis->setLine(dk->pos().x() + PLAYER_SIZE/2, PLAYER_POSY, dk->pos().x() + PLAYER_SIZE/2, TOP_LINE_HEIGHT);
    playerAxis->setVisible(true);
}

/**
 * @brief View::gamePlaying : the player has hit the 'enter' key. The bananas are thrown one by one with the direction that the player chose
 * We also start the QTimer that monitors the game (and it asks that with the controler) and that detects collisions
 */
void View::gamePlaying()
{
    // make the axis disappear and its direction is stored
    playerAxis->setVisible(false);
    qreal angle = playerAxis->rotation();

    // computing the direction-vector for the bananas
    qreal dirRadian = (AXIS_OFFSET - angle) * M_PI / 180;
    qreal dirX = BANANAS_SPEED * cos(dirRadian);
    qreal dirY = -BANANAS_SPEED * sin(dirRadian);

    // passing the direction to the bananas
    foreach (Banana * ban, bananas) {
        ban->setDirection(dirX, dirY);
    }

    // throwing them
    if (bananas.size() > 1) {
        bananaLauncherFlag = true;
        indexOfBananaThrower = 1;
        QTimer * launcher = new QTimer();
        connect(launcher, SIGNAL(timeout()), this, SLOT(thrower()));
        launcher->start(THROWING_INTERVAL);
    }

    bananas.first()->throwing();

    // detect the collision
    refreshTimer->start();

    // detect the end of level
    monitoringTimer->start();
}

/**
 * @brief View::checkLevel : checks if the level was completed perfectly (eg all the block have been destroyed)
 * @return : true if perfect level, false otherwise
 */
bool View::checkPerfectLevel() const
{
    foreach (QGraphicsItem * item, scene->items()) {
        if (Block * block = dynamic_cast<Block*>(item))
            return false;
    }
    return true;
}

/**
 * @brief View::lowerBlocks : get the blocks one step down for the next level
 * @return  : true if the lowest block is not too low (bottom_line)
 */
bool View::lowerBlocks() const
{
    blocks->setPos(blocks->scenePos()+=QPointF(0, 50));
    if (2 * TOP_LINE_HEIGHT + blocks->boundingRect().height() >= BOTTOM_LINE_HEIGHT)
        return false;
    return true;
}

/**
 * @brief View::repositionPlayer : reposition the player where the first banana of the list hit the floor (not necessarly the first one to actually hit but eh ...)
 */
void View::repositionPlayer()
{
    foreach (Banana * ban , bananas) {
        ban->setPos(bananas.first()->scenePos().x(), PLAYER_POSY);
    }
    dk->setPos(bananas.first()->scenePos().x() - PLAYER_SIZE/4, PLAYER_POSY);
}

/**
 * @brief View::incScoreBoard : update the score board
 */
void View::incScoreBoard()
{
    scoreItem->setPlainText("Score : " + QString::number(dk->getScore()));
}

/**
 * @brief View::incNbBlockDestrBoard : update the number of block destroyed
 */
void View::incNbBlockDestrBoard()
{
    blockDestrItem->setPlainText("Nombre de Blocks Détruits : " + QString::number(dk->getNbBlockDestroyed()));
}

/**
 * @brief View::incNbBananasBoard : update the number of bananas that the player can dispose of
 */
void View::incNbBananasBoard()
{
    bananaItem->setPlainText("Bananes : " + QString::number(dk->getNbBananas()));
}

/**
 * @brief View::gameOver : display a msgbox that will present the score
 */
void View::gameOver()
{
    QMessageBox msgBox;
    msgBox.setWindowTitle("Game Over");
    msgBox.setMinimumWidth(GAME_OVER_WIDTH);
    msgBox.setText("Your score : " + QString::number(dk->getScore()) +".");
    msgBox.setInformativeText("Your last checkpoint was level : " + QString::number(dk->getLastCheckpoint()) +".");
    msgBox.exec();
}

/**
 * @brief View::getPlayerName : asks the name of the player
 * @return : QString
 */
const QString View::getPlayerName()
{
    bool ok;
    QString playerName = QInputDialog::getText(this, "HighScores Board", "Please enter your name", QLineEdit::Normal, QString(), &ok);

    if (ok && !playerName.isEmpty())
        return playerName;
    else
        return "Unnamed Player";
}

/**
 * @brief View::displayHighScores : display the highscores saved in a '.dat' file
 * @param highScores : highscores data retrieved via the model
 */
void View::displayHighScores(const QVector<Qhighscore> highScores)
{
    QGraphicsView * gameView = new QGraphicsView(scene, this);
    gameView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    gameView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    gameView->setFixedSize(VIEW_WIDTH, VIEW_HEIGHT);
    scene->setSceneRect(0, 0, VIEW_WIDTH, VIEW_HEIGHT);
    gameView->setBackgroundBrush(QBrush(QPixmap(":/img/res/bck_img_game.png")));

    gameView->show();

    QUiLoader loader;

    QFile file(":/form/highscore.ui");
    file.open(QFile::ReadOnly);

    QWidget * formWidget = loader.load(&file, this);
    file.close();

    for (int i = 0; i < 5; ++i) {
        QLabel * labelName = formWidget->findChild<QLabel*>("label_"+QString::number(i));
        labelName->setText(highScores[i].playerName);
        QLabel * labelScore = formWidget->findChild<QLabel*>("label_"+QString::number(i)+QString::number(i));
        labelScore->setText(QString::number(highScores[i].score));
    }

    QPushButton * pushbutton = formWidget->findChild<QPushButton*>("pushButton");
    connect(pushbutton, SIGNAL(clicked(bool)), this, SLOT(setupui()));

    formWidget->show();
}

/**
 * @brief View::cleanUpGame : remove the bananas and the blocks from the scene / memory
 */
void View::cleanUpGame()
{
    // remove the blocks
    foreach(QGraphicsItem * item, scene->items()) {
        if (typeid(*item) == typeid(Block)) {
            blocks->removeFromGroup(item);
            scene->removeItem(item);
            delete item;
        }
    }
    //remove the bananas
    foreach (Banana * ban, bananas) {
        scene->removeItem(ban);
        delete ban;
    }
    bananas.clear();

}

/**
 * @brief View::playerLoadCheckpoint : ask the player wheter or not to load the last checkpoint
 * @return : false if the player does not want to load the las checkpoint, true otherwise.
 */
bool View::playerLoadCheckpoint()
{
    QString str = "Voulez-vous chargez le dernier checkpoint ("+QString::number(dk->getLastCheckpoint())+") ?";
    const int reply = QMessageBox::question(this, "Game Over", str, QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);

    switch (reply) {
    case QMessageBox::Yes:
        return true;
    case QMessageBox::No:
        return false;
    default:
        return true;
    }
}

/**
 * @brief View::setupui : loads the main menu ui
 */
void View::setupui()
{
    ui->setupUi(this);
}

//////*** SLOTS ***//////
/**
 * @brief View::on_pushButton_clicked : slot for the "Play" button
 */
void View::on_pushButton_clicked()
{
    this->control->setupGame();
}

/**
 * @brief View::playerAxisLeanRight : rotate the axis by one degree clockwise
 */
void View::playerAxisLeanRight()
{
    playerAxis->setTransformOriginPoint(dk->pos().x() + PLAYER_SIZE/2, PLAYER_POSY);
    if (playerAxis->rotation() > MAX_ROTATION) {
        playerAxis->setRotation(-MAX_ROTATION);
    } else
        playerAxis->setRotation(playerAxis->rotation() + 1);
}

/**
 * @brief View::playerAxisLeanLeft : rotate the axis by one degree anti-clockwise
 */
void View::playerAxisLeanLeft()
{
    playerAxis->setTransformOriginPoint(dk->pos().x() + PLAYER_SIZE/2, PLAYER_POSY);
    if (playerAxis->rotation() < -MAX_ROTATION) {
        playerAxis->setRotation(MAX_ROTATION);
    } else
        playerAxis->setRotation(playerAxis->rotation() - 1);
}

/**
 * @brief View::startPlaying : start  the level
 */
void View::startPlaying()
{
    // level is launched, clear focus so the item don't handle the inputs anymore
    scene->clearFocus();

    // the bananas are thrown
    gamePlaying();
}

/**
 * @brief View::collision : this is where it gets ugly. So many 'if'
 * Basically we first check the position of the bananas and the edges of the scene - that's easy
 * Then we check with the block positions :
 *      - if the block is a bonus block, get the bonus then continues with the same direction
 *      - otherwise we check if the banana collides with the top/bottom sides or the left/right sides
 */
void View::collision()
{
//    QElapsedTimer timer;
//    timer.start();
    // detection of collision with the edges of the playground
    foreach (Banana * ban, bananas) {
        // left edge
        if (ban->scenePos().x() + ban->getDirection().x() < 0) {
            ban->setDirection(-ban->getDirection().x(), ban->getDirection().y());
        } // right edge
        else if (ban->scenePos().x() + BANANA_SIZE + ban->getDirection().x() > VIEW_WIDTH) {
            ban->setDirection(-ban->getDirection().x(), ban->getDirection().y());
        } // top edge
        else if (ban->scenePos().y() + ban->getDirection().y() < TOP_LINE_HEIGHT) {
            ban->setDirection(ban->getDirection().x(), -ban->getDirection().y());
        } // bottom edge
        else if (ban->scenePos().y() + BANANA_SIZE/2 + ban->getDirection().y() > BOTTOM_LINE_HEIGHT) {
            ban->crash();
        } // block collisions
        else if (!(ban->collidingItems().isEmpty())) {
            if (typeid(*(ban->collidingItems().first())) == typeid(Block)) {
                Block * brik = dynamic_cast<Block*>(ban->collidingItems().first());
                if (brik->getBonusType() != 0) {
                    switch (brik->getBonusType()) {
                    case MORE_BANANA_BONUS:
                        blocks->removeFromGroup(brik);
                        scene->removeItem(brik);
                        delete brik;
                        dk->setNbBananas();
                        incNbBananasBoard();
                        break;
                    default:
                        break;
                    }
                } else {
                    // top / bottom block collision
                    if (ban->pos().y() + BANANA_SIZE > brik->scenePos().y() + BLOCK_SIZE || ban->pos().y() < brik->scenePos().y()) {
                        ban->setDirection(ban->getDirection().x(), -ban->getDirection().y());
                        if (brik->decPoints()) {
                            blocks->removeFromGroup(brik);
                            scene->removeItem(brik);
                            delete brik;
                            dk->setNbBlockDestroyed();
                            incNbBlockDestrBoard();
                        }
                    }
                    // right / left block collision
                    else if (ban->pos().x() < brik->scenePos().x() || ban->pos().x() + BANANA_SIZE > brik->scenePos().x() + BLOCK_SIZE) {
                        ban->setDirection(-ban->getDirection().x(), ban->getDirection().y());
                        if (brik->decPoints()) {
                            blocks->removeFromGroup(brik);
                            scene->removeItem(brik);
                            delete brik;
                            dk->setNbBlockDestroyed();
                            incNbBlockDestrBoard();
                        }
                    }
                }
            }
        }
    }
//    qDebug()<<timer.nsecsElapsed();
//     max : 100 microseconds (PC) /
    return;
}

/**
 * @brief View::monitorLevel : asks the controler whether all the bananas have crashed to detect end of level
 */
void View::monitorLevel()
{
    // false = game continues   -   true = all the bananas crashed
    if (this->control->monitorLevel(bananas)) {
        monitoringTimer->stop();
        refreshTimer->stop();
        this->control->gameCore();
    }
}

/**
 * @brief View::thrower : throws one bananas every THROWING_INTERVAL milliseconds
 */
void View::thrower()
{
    if (bananaLauncherFlag) {
        bananas[indexOfBananaThrower]->throwing();
        indexOfBananaThrower++;
        if (indexOfBananaThrower >= bananas.size()) {
            bananaLauncherFlag = false;
            QTimer * timerSender = qobject_cast<QTimer *>(sender());
            if (timerSender)
                timerSender->stop();
        }
    }
}

/**
 * @brief View::on_pushButton_4_clicked : close the game
 */
void View::on_pushButton_4_clicked()
{
    this->close();
}

void View::on_pushButton_2_clicked()
{
    QString dir = QFileDialog::getOpenFileName(this, tr("Charger une partie sauvée"), "./", tr(".dat Files (*.dat)"));
    control->openGame(dir);
}

void View::on_pushButton_3_clicked()
{
    this->control->displayHighScores();
}
