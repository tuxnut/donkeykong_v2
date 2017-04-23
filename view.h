#ifndef VIEW_H
#define VIEW_H

#include "block.h"
#include "player.h"
#include "unistd.h"
#include "banana.h"
#include "gamesettings.h"

#include <QMainWindow>
#include <QGraphicsView>
#include <QGraphicsItemGroup>
#include <QTimer>
#include <typeinfo>
#include <QFileDialog>
#include <QMessageBox>
#include <QInputDialog>
#include <QtUiTools>

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
    void gameOver();
    const QString getPlayerName();
    void displayHighScores(const QVector<Qhighscore> highScores);

private slots:
    void setupui();
    void on_pushButton_clicked();
    void playerAxisLeanRight();
    void playerAxisLeanLeft();
    void startPlaying();
    void collision();
    void monitorLevel();
    void thrower();
    void on_pushButton_4_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();

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
