#include "view.h"
#include "ui_view.h"
#include "coregame.h"

#include <QDebug>

View::View(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::View)
{
    ui->setupUi(this);
    scene = new QGraphicsScene(this);
    qDebug() << scene->focusItem();
    refreshTimer = new QTimer();
    connect(this->refreshTimer, SIGNAL(timeout()), this, SLOT(collision()));
    refreshTimer->setInterval(REFRESH_COLLISION);
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

    playerAxis = new QGraphicsLineItem(PLAYER_POSX + PLAYER_SIZE/2, PLAYER_POSY, PLAYER_POSX + PLAYER_SIZE/2, TOP_LINE_HEIGHT);
    playerAxis->setVisible(false);
    scene->addItem(playerAxis);
    gameView->show();
    usleep(1000000);
}

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
        block->setPos(blockSettings[i], SPAWNING_LINE);
        scene->addItem(block);
    }
}

void View::playerAction()
{
    scene->clearFocus();
    qDebug() << dk->hasFocus();
    scene->setFocusItem(dk);
    qDebug() << dk->hasFocus();
    playerAxis->setVisible(true);
}

void View::gamePlaying()
{
    // make the axis disappear and retrieve its direction
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
}

/*** SLOTS ***/
void View::on_pushButton_clicked()
{
    this->control->setupGame();
}

void View::playerAxisLeanRight()
{
    playerAxis->setTransformOriginPoint(PLAYER_POSX + PLAYER_SIZE/2, PLAYER_POSY);
    if (playerAxis->rotation() > MAX_ROTATION) {
        playerAxis->setRotation(-MAX_ROTATION);
    } else
        playerAxis->setRotation(playerAxis->rotation() + 1);
}

void View::playerAxisLeanLeft()
{
    playerAxis->setTransformOriginPoint(PLAYER_POSX + PLAYER_SIZE/2, PLAYER_POSY);
    if (playerAxis->rotation() < -MAX_ROTATION) {
        playerAxis->setRotation(MAX_ROTATION);
    } else
        playerAxis->setRotation(playerAxis->rotation() - 1);
}

void View::startPlaying()
{
    // game is launched, clear focus so the item don't handle the inputs anymore
    scene->clearFocus();

    // setting up timer and stuff
    gamePlaying();
}

void View::collision()
{
    // detection of collision with the edges of the playground
    foreach (Banana * ban, bananas) {
        if (ban->scenePos().x() + ban->getDirection().x() < 0) {
            ban->setDirection(-ban->getDirection().x(), ban->getDirection().y());
        } else if (ban->scenePos().x() + BANANA_SIZE + ban->getDirection().x() > VIEW_WIDTH) {
            ban->setDirection(-ban->getDirection().x(), ban->getDirection().y());
        } else if (ban->scenePos().y() + ban->getDirection().y() < TOP_LINE_HEIGHT) {
            ban->setDirection(ban->getDirection().x(), -ban->getDirection().y());
        } else if (ban->scenePos().y() + BANANA_SIZE/2 + ban->getDirection().y() > BOTTOM_LINE_HEIGHT) {
            ban->crash();
        } else if (!(ban->collidingItems().isEmpty())) {
            if (typeid(*(ban->collidingItems().first())) == typeid(Block)) {
                switch (ban->collidingItems().first()->collisionLocation(ban->pos(), ban->getDirection())) {
                case value:

                    break;
                default:
                    break;
                }
            }

            return;
        }
    }
}
