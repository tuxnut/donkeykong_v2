#ifndef MODEL_H
#define MODEL_H

#include "player.h"

#include <QFile>
#include <QJsonObject>
#include <QJsonDocument>

class Player;

class Model
{
public:
    Model();
    bool saveGameAuto(Player *p);
    bool loadPlayer(const QString &dir, Player * p);
};

#endif // MODEL_H
