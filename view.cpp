#include "view.h"
#include "ui_view.h"
#include "coregame.h"

View::View(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::View)
{
    ui->setupUi(this);
    scene = new QGraphicsScene(this);
}

View::~View()
{
    delete ui;
}

void View::setControl(CoreGame *control)
{
    this->control = control;
}

void View::displayGame(Player *dk)
{
    // setting up the QgraphicsView / Scene
    QGraphicsView * gameView = new QGraphicsView(scene, this);
    gameView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    gameView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    gameView->setFixedSize(VIEW_WIDTH, VIEW_HEIGHT);
    scene->setSceneRect(0, 0, VIEW_WIDTH, VIEW_HEIGHT);

    gameView->setBackgroundBrush(QBrush(QPixmap(":/img/res/bck_img_game.png")));  // image de bckgr dans les ressources !!!!!
    QGraphicsLineItem * topLine = new QGraphicsLineItem(0, TOP_LINE_HEIGHT, VIEW_WIDTH, TOP_LINE_HEIGHT);
    QGraphicsLineItem * bottomLine = new QGraphicsLineItem(0, BOTTOM_LINE_HEIGHT, VIEW_WIDTH, BOTTOM_LINE_HEIGHT);
    scene->addItem(topLine);
    scene->addItem(bottomLine);

    // adding the player
    dk->setPos(PLAYER_POSX, PLAYER_POSY);
    dk->setFlag(QGraphicsItem::ItemIsFocusable);
    dk->setFocus();
    scene->addItem(dk);

    gameView->show();
}

void View::displayLevel()
{
    QList <int *> blockSettings = this->control->setupLevel();

    if (blockSettings.size() == 0)
        return;
//        this->control->updateGame(); // TODO !!

    for (int i = 0; i < blockSettings.size(); i++) {
        Block * block = new Block(blockSettings[i][0]);
        block->setPos(blockSettings[i][1], SPAWNING_LINE);
        scene->addItem(block);
    }
}

void View::on_pushButton_clicked()
{
    this->control->setupGame();
}
