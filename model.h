#ifndef MODEL_H
#define MODEL_H

#include "player.h"
#include "coregame.h"
#include "gamesettings.h"

#include <QFile>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>

class Player;
class CoreGame;

class Model
{
private:
    CoreGame * control;

public:
    Model();
    void setControl(CoreGame * control);
    bool saveGameAuto(Player *p) const;
    bool loadPlayer(Player * p, const QString &dir = "saver.dat") const;
    bool isHighScore(const int &score) const;
    const QVector<Qhighscore> getHighScores();
};

#endif // MODEL_H
