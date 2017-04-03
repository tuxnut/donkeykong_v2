#ifndef VIEW_H
#define VIEW_H

#include <QMainWindow>
#include <QGraphicsView>
#include "block.h"
#include "player.h"

#define VIEW_HEIGHT 800
#define VIEW_WIDTH 600
#define BOTTOM_LINE_HEIGHT 750
#define TOP_LINE_HEIGHT 50
#define PLAYER_POSX 275
#define PLAYER_POSY 700

class CoreGame;

namespace Ui {
class View;
}

class View : public QMainWindow
{
    Q_OBJECT

public:
    explicit View(QWidget *parent = 0);
    ~View();

    void setControl(CoreGame *control);
    void displayGame(Player *dk);

private slots:
    void on_pushButton_clicked();

private:
    Ui::View *ui;
    CoreGame *control;
    QGraphicsScene *scene;
};

#endif // VIEW_H
