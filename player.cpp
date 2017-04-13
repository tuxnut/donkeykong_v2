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

int Player::getNbBananas() const
{
    return this->bananas;
}

int Player::getScore() const
{
    return this->score;
}

int Player::getNbBlockDestroyed() const
{
    return this->blockDestroyed;
}

void Player::setNbBananas(const int nbBananas)
{
    if (nbBananas != 0)
        this->bananas = nbBananas;
    else
        this->bananas++;
}

void Player::setScore(const int score)
{
    if (score != 0)
        this->score = score;
    else
        this->score++;
}

void Player::setNbBlockDestroyed(const int nbBlocks)
{
    if (nbBlocks != 0)
        this->blockDestroyed = nbBlocks;
    else
        this->blockDestroyed++;
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
        emit leanRight(); // rotate the playerAxis to the right
        break;
    case Qt::Key_Left:
        emit leanLeft(); // rotate the playerAxis to the left
        break;
    case Qt::Key_Enter:
        emit launch(); // calls view.startPlaying() / stops the focus (inputs no longer handled)
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
