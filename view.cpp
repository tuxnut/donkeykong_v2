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
    connect(dk, SIGNAL(launch()), this, SLOT(playerAxisLeanRight()));

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
