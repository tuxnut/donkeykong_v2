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

/**
 * @brief The Model class deals with I/O interfaces. Most of the files dealt with are binary written JSON files.
 */
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
