#include "banana.h"

Banana::Banana(QObject *parent) : QObject(parent)
{
    // set default pixmap
    setPixmap(QPixmap(":/img/res/img_banana_simple.png"));
}
