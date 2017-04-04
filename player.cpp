#include "player.h"

#include <QDebug>

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
    qDebug() << event->key();
    switch (event->key()) {
    case Qt::Key_Right:
        qDebug() << "right";
        break;
    case Qt::Key_Left:
        qDebug() << "left";
        break;
    case Qt::Key_Space:
        qDebug() << "space";
        break;
    default:
        break;
    }
}

void Player::changeFrame()
{
    if (!source)
        return;
    setPixmap(source->currentPixmap());
}
