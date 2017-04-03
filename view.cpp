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

    dk->setPos(PLAYER_POSX, PLAYER_POSY);
    scene->addItem(dk);

    gameView->show();
}

void View::on_pushButton_clicked()
{
    this->control->setupGame();
}
