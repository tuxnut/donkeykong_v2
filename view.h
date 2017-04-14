#ifndef VIEW_H
#define VIEW_H

#include "block.h"
#include "player.h"
#include "unistd.h"
#include "banana.h"

#include <QMainWindow>
#include <QGraphicsView>
#include <QGraphicsItemGroup>
#include <QTimer>
#include <typeinfo>

/*** Basically game settings ***/
#define VIEW_HEIGHT 800
#define VIEW_WIDTH 600
#define BOTTOM_LINE_HEIGHT 750
#define TOP_LINE_HEIGHT 50
#define BLOCK_DESTR_POSX 425
#define PLAYER_POSX 275
#define PLAYER_POSY 700
#define PLAYER_SIZE 50
#define BANANA_SIZE 20
#define MAX_ROTATION 80 // of the axis while rotating
#define AXIS_OFFSET 90  // since its vertical at the initialization
#define BANANAS_SPEED 4 // in px per frame (if possible 60 fps)
#define REFRESH_COLLISION 17 // match the FPS
#define MONITORING_INTERVAL 150
#define THROWING_INTERVAL 200
#define MORE_BANANA_BONUS 1
#define PADDLE_BONUS 2
#define MORE_LIFE_BANANA_BONUS 3

class CoreGame;
class Player;

namespace Ui {
class View;
}

class View : public QMainWindow
{
    Q_OBJECT

public:
    explicit View(QWidget *parent = 0);
    ~View();
    Player * getPlayer();
    int getNbPixmapBanana();
    void setControl(CoreGame *control);
    void displayGame(Player *dk);
    void displayLevel();
    void playerAction();
    void gamePlaying();
    bool checkPerfectLevel() const;
    bool lowerBlocks() const;
    void repositionPlayer();
    void incScoreBoard();
    void incNbBlockDestrBoard();
    void incNbBananasBoard();

private slots:
    void on_pushButton_clicked();
    void playerAxisLeanRight();
    void playerAxisLeanLeft();
    void startPlaying();
    void collision();
    void monitorGame();
    void thrower();
    void on_pushButton_4_clicked();

private:
    bool bananaLauncherFlag;
    int indexOfBananaThrower;
    QTimer * refreshTimer;
    QTimer * monitoringTimer;
    QGraphicsScene * scene;
    QList <Banana *> bananas; // this is a graphics element (derived from QGraphicsPixmapItems)
    Player * dk; // this is also a grpahics element (derived from QGraphicsPixmapItems)
    QGraphicsLineItem * playerAxis;
    QGraphicsTextItem * scoreItem;
    QGraphicsTextItem * blockDestrItem;
    QGraphicsTextItem * bananaItem;
    QGraphicsItemGroup * blocks;
    Ui::View * ui;
    CoreGame * control;
};

#endif // VIEW_H
