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

    dk = nullptr;
    paddle = false;
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
 * @brief View::getPaddleState : return true if the paddle is still displaying, false otherwise
 * @return
 */
bool View::getPaddleState()
{
    return paddle;
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

    // hide the menu
    ui->centralWidget->setVisible(false);

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

    // adding the paddle bonus
    pad = new Paddle();
    pad->setFlag(QGraphicsItem::ItemIsFocusable);

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
    int christopher = this->control->updateNbBananas();
    for (int i = 0; i < christopher; i++) {
        bananas.append(new Banana());
        bananas.last()->setPos(PLAYER_POSX + PLAYER_SIZE/4, PLAYER_POSY);
        scene->addItem(bananas.last());
    }

    // ask the controler what to add to the view
    QVector<blockSettings*> clemence = this->control->setupLevel();

    if (clemence.isEmpty())
        return;

    for (int i = 0; i < clemence.size(); i++) {
//        Block * block = new Block(clemence[i]->point);

        switch (clemence[i]->bonusType) {
        case 0:
        {
            Block * block = new Block(clemence[i]->point);
            block->setPos(clemence[i]->posX, TOP_LINE_HEIGHT);
            blocks->addToGroup(block);
            break;
        }
        case MORE_BANANA_BONUS:
        {
            BonusBlock * bonblock = new BonusBlock(MORE_BANANA_BONUS);
            bonblock->setPos(clemence[i]->posX, TOP_LINE_HEIGHT + BLOCK_SIZE/4);
            blocks->addToGroup(bonblock);
            break;
        }
        case PADDLE_BONUS:
        {
            BonusBlock * bonblock = new BonusBlock(PADDLE_BONUS);
            bonblock->setPos(clemence[i]->posX, TOP_LINE_HEIGHT + BLOCK_SIZE/4);
            blocks->addToGroup(bonblock);
            break;
        }
        case MORE_LIFE_BANANA_BONUS:
        {
            BonusBlock * bonblock = new BonusBlock(MORE_LIFE_BANANA_BONUS);
            bonblock->setPos(clemence[i]->posX, TOP_LINE_HEIGHT + BLOCK_SIZE/4);
            blocks->addToGroup(bonblock);
            break;
        }
        default:
//            block->setPos(clemence[i]->posX, TOP_LINE_HEIGHT);
            break;
        }
    }
}

/**
 * @brief View::playerAction : let the player choose a direction to send the bananas. We change to an asynchronous behavior from here. It's also the player that handles the keyboard inputs.
 */
void View::playerAction()
{
    scene->clearFocus();
    scene->setFocusItem(dk);
    playerAxis->setLine(dk->pos().x() + PLAYER_SIZE/2, PLAYER_POSY, dk->pos().x() + PLAYER_SIZE/2, AXIS_TOP);
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
    foreach (QGraphicsItem * jonathan, scene->items()) {
        if (Block * block = dynamic_cast<Block*>(jonathan))
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
    // hide the menu interface
    ui->centralWidget->setVisible(false);

    // load an ui file (as the ui file is in the ressource file (so it's built within the sources) the vieww handle the loading)
    QUiLoader loader;
    QFile file(":/form/highscore.ui");
    file.open(QFile::ReadOnly);

    QWidget * chloe = loader.load(&file, this);
    file.close();

    // set the background
    chloe->setStyleSheet("background-image: url(:/img/res/bck_img_game.png)");

    // sets up the QLabel with the highscores
    for (int i = 0; i < 5; ++i) {
        QLabel * labelName = chloe->findChild<QLabel*>("label_"+QString::number(i));
        labelName->setText(highScores[i].playerName);
        QLabel * labelScore = chloe->findChild<QLabel*>("label_"+QString::number(i)+QString::number(i));
        labelScore->setText(QString::number(highScores[i].score));
    }

    // return button to the main menu
    QPushButton * pushbutton = chloe->findChild<QPushButton*>("pushButton");
    connect(pushbutton, SIGNAL(clicked(bool)), this, SLOT(setupui()));

    chloe->show();
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
    QString str;
    int reply;

    if (dk->getLastCheckpoint() > 0) {
        str = "Voulez-vous chargez le dernier checkpoint ("+QString::number(dk->getLastCheckpoint())+") ?";
        reply = QMessageBox::question(this, "Game Over", str, QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
    }

    switch (reply) {
    case QMessageBox::Yes:
        return true;
    case QMessageBox::No:
        return false;
    default:
        return false;
    }
}

/**
 * @brief View::closeEvent : clean up the memory used by the program
 * @param event
 */
void View::closeEvent(QCloseEvent * event)
{
    int reply;
    if ((dk != nullptr) && dk->getLastCheckpoint() != dk->getScore()) {
        QString str = "Votre dernier checkpoint est au niveau "+ QString::number(dk->getLastCheckpoint()) +". Voulez-vous tout de meme fermer l'application ?";
        reply = QMessageBox::question(this, "Fermeture", str, QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
    }

    switch (reply) {
    case QMessageBox::Yes:
        scene->clear();
        delete refreshTimer;
        delete monitoringTimer;
        control->closeCleanup();
        event->accept();
        break;
    case QMessageBox::No:
        event->ignore();
        break;
    default:
        event->accept();
        break;
    }
}

/**
 * @brief View::addPaddleBonus : add a paddle that the player can move to hit the bananas
 */
void View::addPaddleBonus()
{
    paddle = true;

    // set a single shot Qtimer that will delete the paddle
    QTimer * paddleTimer = new QTimer();
    paddleTimer->setSingleShot(true);
    paddleTimer->setInterval(LIFESPAN_PADDLE);
    connect(paddleTimer, SIGNAL(timeout()), this, SLOT(removePaddle()));

    //add the paddle to the scene
    pad->setPos(PADDLE_POSX, PADDLE_POSY);
    scene->addItem(pad);
    scene->setFocusItem(pad);

    paddleTimer->start();
}

/////////*** SLOTS ***/////////

/**
 * @brief View::setupui : loads the main menu ui
 */
void View::setupui()
{
    QPushButton * pushButton = qobject_cast<QPushButton*>(sender());
    if (pushButton) {
        pushButton->parentWidget()->deleteLater();
        control->changeToMenuSound();
        ui->centralWidget->setVisible(true);
    }
}

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
 *      - otherwise we (ie the controler) check if the banana collides with the top/bottom sides or the left/right sides
 * An effort has been made to put the vast majority of the calls to functions and methods in this function as
 * inline, letting the optimizer do his work (whether or not putting the lines codes directly in this method)
 */
void View::collision()
{
//    QElapsedTimer timer;
//    timer.start();
    // detection of collision with the edges of the playground
    foreach (Banana * ban, bananas) {
        // left edge
        if (ban->scenePos().x() + ban->getDirectionToPointF().x() < 0) {
            ban->setDirection(-ban->getDirectionToPointF().x(), ban->getDirectionToPointF().y());
        } // right edge
        else if (ban->scenePos().x() + BANANA_SIZE + ban->getDirectionToPointF().x() > VIEW_WIDTH) {
            ban->setDirection(-ban->getDirectionToPointF().x(), ban->getDirectionToPointF().y());
        } // top edge
        else if (ban->scenePos().y() + ban->getDirectionToPointF().y() < TOP_LINE_HEIGHT) {
            ban->setDirection(ban->getDirectionToPointF().x(), -ban->getDirectionToPointF().y());
        } // bottom edge
        else if (ban->scenePos().y() + BANANA_SIZE/2 + ban->getDirectionToPointF().y() > BOTTOM_LINE_HEIGHT) {
            ban->crash();
        } // block collisions
        else if (!(ban->collidingItems().isEmpty())) {
            if (typeid(*(ban->collidingItems().first())) == typeid(Block)) {
                Block * brik = dynamic_cast<Block*>(ban->collidingItems().first());

//                // top / bottom block collision
//                if (ban->pos().y() + BANANA_SIZE > brik->scenePos().y() + BLOCK_SIZE || ban->pos().y() < brik->scenePos().y()) {
//                    ban->setDirection(ban->getDirectionToPointF().x(), -ban->getDirectionToPointF().y());
//                    if (brik->decPoints()) {
//                        blocks->removeFromGroup(brik);
//                        scene->removeItem(brik);
//                        delete brik;
//                        dk->setNbBlockDestroyed();
//                        incNbBlockDestrBoard();
//                    }
//                }
//                // right / left block collision
//                else if (ban->pos().x() < brik->scenePos().x() || ban->pos().x() + BANANA_SIZE > brik->scenePos().x() + BLOCK_SIZE) {
//                    ban->setDirection(-ban->getDirectionToPointF().x(), ban->getDirectionToPointF().y());
//                    if (brik->decPoints()) {
//                        blocks->removeFromGroup(brik);
//                        scene->removeItem(brik);
//                        delete brik;
//                        dk->setNbBlockDestroyed();
//                        incNbBlockDestrBoard();
//                    }
//                }

                ban->setDirection(control->computeAngle(ban->scenePos(), brik->scenePos(), ban->getDirection()));
                if (brik->decPoints()) {
                    blocks->removeFromGroup(brik);
                    scene->removeItem(brik);
                    delete brik;
                    dk->setNbBlockDestroyed();
                    incNbBlockDestrBoard();
                }
            } else if (typeid(*(ban->collidingItems().first())) == typeid(BonusBlock)) {
                BonusBlock * gBlanc = dynamic_cast<BonusBlock*>(ban->collidingItems().first());

                // bonus block collision
                switch (gBlanc->getBonusType()) {
                case MORE_BANANA_BONUS:
                    blocks->removeFromGroup(gBlanc);
                    scene->removeItem(gBlanc);
                    delete gBlanc;
                    dk->setNbBananas();
                    incNbBananasBoard();
                    break;
                case PADDLE_BONUS:
                    blocks->removeFromGroup(gBlanc);
                    scene->removeItem(gBlanc);
                    delete gBlanc;
                    addPaddleBonus();
                    break;
                case MORE_LIFE_BANANA_BONUS:
                    blocks->removeFromGroup(gBlanc);
                    scene->removeItem(gBlanc);
                    delete gBlanc;
                    break;
                default:
                    break;
                }
             } else if (typeid(*(ban->collidingItems().first())) == typeid(Paddle)) {
                ban->setDirection(ban->getDirectionToPointF().x(), -ban->getDirectionToPointF().y());
            }
        }
    }
//    qDebug()<<timer.nsecsElapsed();
//     max : 200 microseconds (PC) /-/ 328,522 microseconds (laptop)
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
 * @brief View::removePaddle : removes the paddle from the scene once its timer has timed out
 */
void View::removePaddle()
{
    if (paddle) {
        paddle = false;
        scene->removeItem(pad);
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

void View::on_pushButton_5_clicked()
{
    QPushButton * button = qobject_cast<QPushButton *>(sender());
    if (button) {
        if (control->changeMusicState()) {
            button->setStyleSheet("background-image: url(:/img/res/soundon.png)");
        } else {
            button->setStyleSheet("background-image: url(:/img/res/soundoff.png)");
        }
    }

}
