#ifndef VIEW_H
#define VIEW_H

#include "block.h"
#include "player.h"
#include "unistd.h"
#include "banana.h"

#include <QMainWindow>
#include <QGraphicsView>
#include <QTimer>
#include <QtMath>

#define VIEW_HEIGHT 800
#define VIEW_WIDTH 600
#define BOTTOM_LINE_HEIGHT 750
#define TOP_LINE_HEIGHT 50
#define PLAYER_POSX 275
#define PLAYER_POSY 700
#define PLAYER_SIZE 50
#define SPAWNING_LINE 100
#define MAX_ROTATION 80 // of the axis while rotating
#define AXIS_OFFSET 90  // since its vertical at the initialization
#define BANANAS_SPEED 4 // in px per frame (if possible 60 fps)
#define FPS 60

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

private slots:
    void on_pushButton_clicked();
    void playerAxisLeanRight();
    void playerAxisLeanLeft();
    void startPlaying();

private:
    QTimer * refreshTimer;
    QGraphicsLineItem *playerAxis;
    QList <Banana *> bananas;
    Ui::View *ui;
    Player *dk;
    CoreGame *control;
    QGraphicsScene *scene;
};

#endif // VIEW_H
