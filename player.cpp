#include "player.h"

Player::Player(QObject *parent) : QObject(parent)
{
    this->score = 0;
    this->bananas = 1;
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

void Player::setNbBananas(int nbBananas)
{
    this->bananas = nbBananas;
}

void Player::setScore(int score)
{
    this->score = score;
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

void Player::changeFrame()
{
    if (!source)
        return;
    setPixmap(source->currentPixmap());
}
