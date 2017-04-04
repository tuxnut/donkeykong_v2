#include "player.h"

#include <QDebug>
#include <QGraphicsScene>

Player::Player(QObject *parent) : QObject(parent)
{
    this->score = 0;
    this->bananas = 1;
    this->blockDestroyed = 0;
    animation(new QMovie(":/animations/res/anim.gif"));
    setScale(scale() / 4);
}

int Player::getNbBananas()
{
    return this->bananas;
}

int Player::getScore()
{
    return this->score;
}

int Player::getNbBlockDestroyed()
{
    return this->blockDestroyed;
}

void Player::setNbBananas(const int nbBananas)
{
    this->bananas = nbBananas;
}

void Player::setScore(const int score)
{
    this->score = score;
}

void Player::setNbBlockDestroyed(const int nbBlocks)
{
    this->blockDestroyed = nbBlocks;
}

void Player::animation(QMovie *source)
{
    // set the source for the animation
    this->source = source;

    // connect the changes of frames with our function that will send the frame (as a QPixmap) to the scene
    // to depict our player
    connect(source, SIGNAL(frameChanged(int)), this, SLOT(changeFrame()));
    source->start();
}

void Player::keyPressEvent(QKeyEvent * event)
{
    switch (event->key()) {
    case Qt::Key_Right:
        qDebug()<<"1";
        emit leanRight();
        break;
    case Qt::Key_Left:
        emit leanLeft();
        break;
    case Qt::Key_Enter:
        qDebug() << "enter";
        break;
    default:
        break;
    }
}

/*** SLOTS ***/

void Player::changeFrame()
{
    if (!source)
        return;
    setPixmap(source->currentPixmap());
}
