#include "view.h"
#include "ui_view.h"
#include "coregame.h"

#include <QDebug>
#include <QElapsedTimer>

View::View(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::View)
{
    ui->setupUi(this);
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
    connect(this->monitoringTimer, SIGNAL(timeout()), this, SLOT(monitorGame()));
}

View::~View()
{
    delete ui;
}

Player *View::getPlayer()
{
    return this->dk;
}

void View::setControl(CoreGame *control)
{
    this->control = control;
}

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
        bananas.at(i)->setPos(this->dk->scenePos());
        scene->addItem(bananas.at(i));
    }

    // ask the controler what to add to the view
    int * blockSettings = this->control->setupLevel();

    if (!blockSettings)
        return;

    for (int i = 1; i < 2 * blockSettings[0]; i += 2) {
        Block * block = new Block(blockSettings[i+1]);
        block->setPos(blockSettings[i], TOP_LINE_HEIGHT);
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
//        ban->setFlag(QGraphicsItem::ItemIsMovable);
        ban->setDirection(dirX, dirY);
        ban->throwing();
    }

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
    foreach (const QGraphicsItem * item, scene->items()) {
        if (typeid(item) == typeid(Block))
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
    for (int i = 0; i < 10; ++i) {
        qDebug()<<blocks->scenePos();
        blocks->setPos(blocks->scenePos()+=QPointF(0, 5));
        qDebug()<<blocks->scenePos();
        usleep(100000);
    }
    return true;
}

void View::repositionPlayer()
{
    for (int i = 0; i < bananas.size(); ++i) {
        bananas[i]->setPos(bananas.first()->scenePos().x(), PLAYER_POSY + PLAYER_SIZE/2);
    }
    dk->setPos(bananas.first()->scenePos().x(), dk->pos().y());
}

/*** SLOTS ***/
/**
 * @brief View::on_pushButton_clicked : slot for the "Play" button
 */
void View::on_pushButton_clicked()
{
    this->control->setupGame();
}

void View::playerAxisLeanRight()
{
    playerAxis->setTransformOriginPoint(dk->pos().x() + PLAYER_SIZE/2, PLAYER_POSY);
    if (playerAxis->rotation() > MAX_ROTATION) {
        playerAxis->setRotation(-MAX_ROTATION);
    } else
        playerAxis->setRotation(playerAxis->rotation() + 1);
}

void View::playerAxisLeanLeft()
{
    playerAxis->setTransformOriginPoint(dk->pos().x() + PLAYER_SIZE/2, PLAYER_POSY);
    if (playerAxis->rotation() < -MAX_ROTATION) {
        playerAxis->setRotation(MAX_ROTATION);
    } else
        playerAxis->setRotation(playerAxis->rotation() - 1);
}

void View::startPlaying()
{
    // level is launched, clear focus so the item don't handle the inputs anymore
    scene->clearFocus();

    // the bananas are thrown
    gamePlaying();
}

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
                // top / bottom block collision
                if (ban->pos().y() + BANANA_SIZE > brik->scenePos().y() + BLOCK_SIZE || ban->pos().y() < brik->scenePos().y()) {
                    ban->setDirection(ban->getDirection().x(), -ban->getDirection().y());
                    if (brik->decPoints()) {
                        blocks->removeFromGroup(brik);
                        scene->removeItem(brik);
                        delete brik;
                        dk->setNbBlockDestroyed(0);
                    }
                }
                // right / left block collision
                else if (ban->pos().x() < brik->scenePos().x() || ban->pos().x() + BANANA_SIZE > brik->scenePos().x() + BLOCK_SIZE) {
                    ban->setDirection(-ban->getDirection().x(), ban->getDirection().y());
                    if (brik->decPoints()) {
                        blocks->removeFromGroup(brik);
                        scene->removeItem(brik);
                        delete brik;
                        dk->setNbBlockDestroyed(0);
                    }
                }
            }
        }
    }
//    qDebug()<<timer.nsecsElapsed();
//     max : 100 microseconds (PC) /
    return;
}

void View::monitorGame()
{
    // false = game continues   -   true = all the bananas crashed
    if (this->control->monitorGame(bananas)) {
        monitoringTimer->stop();
        refreshTimer->stop();
        this->control->gameCore();
    }
}

void View::on_pushButton_4_clicked()
{
    this->close();
}
