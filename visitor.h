#ifndef VISITOR_H
#define VISITOR_H

#include "player.h"

#include <QFile>
#include <QJsonObject>
#include <QJsonDocument>

class Player;
class Model;

class Visitor
{
private:
    Model * model;

public:
    Visitor(Model *model);
    void savingVisit(Player * p);
};

#endif // VISITOR_H
