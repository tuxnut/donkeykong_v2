#include "model.h"

#include <QDebug>

Model::Model()
{

}

void Model::setControl(CoreGame *control)
{
    this->control = control;
}

/**
 * @brief Model::saveGameAuto : saves the game on a '.dat' file every time the player has realised a perfect level
 * @param p : instance of player
 * @return  : true if writing succeeded, false otherwise
 */
bool Model::saveGameAuto(Player * p) const
{
    QJsonObject obj;
    obj["Score"] = p->getScore();
    obj["NbBananas"] = p->getNbBananas();
    obj["NbBlocksDestroyed"] = p->getNbBlockDestroyed();

    QJsonDocument doc(obj);

    QFile file("saver.dat");
    if (!file.open(QIODevice::WriteOnly)) {
        qWarning("couldn't create saving file");
        return false;
    }

    file.write(doc.toBinaryData());
    file.close();

    return true;
}

/**
 * @brief Model::loadPlayer : will load a player from a '.dat' file
 * @param dir : path to the file to load
 * @param p : instance of player
 * @return : true if loading succeeded, false otherwise
 */
bool Model::loadPlayer(Player *p, const QString &dir) const
{
    if (!QFile::exists(dir))
        return false;

    QFile f(dir);
    if (!f.open(QIODevice::ReadOnly))
        return false;

    QJsonDocument doc = QJsonDocument::fromBinaryData(f.readAll());
    if (doc.isNull())
        return false;

    f.close();
    QJsonObject obj = doc.object();

    p->setScore(obj["Score"].toInt());
    p->setNbBananas(obj["NbBananas"].toInt());
    p->setNbBlockDestroyed(obj["NbBlocksDestroyed"].toInt());
    return true;
}

/**
 * @brief Model::isHighScore : determine if the score is an high score and will write on the highscores file accordingly
 * @param score : the score of the player
 * @return  : true if writing succeeded, false otherwise
 */
bool Model::isHighScore(const int &score) const
{
    QFile file("../DonkeyKongv2/res/highscore.dat");
    if (!file.open(QIODevice::ReadOnly))
        return false;

    QJsonDocument doc = QJsonDocument::fromBinaryData(file.readAll());
    if (doc.isNull())
        return false;
    file.close();

    QJsonArray highscore = doc.array();
    int index = 0;

    foreach (const QJsonValue &val, highscore) {
        QJsonObject player = val.toObject();
        if (score > player["Score"].toInt()) {
            const QString &playerName = this->control->getPlayerName();

            QJsonObject newPlayer;
            newPlayer["Name"] = playerName;
            newPlayer["Score"] = score;
            highscore.insert(index, QJsonValue(newPlayer));
            highscore.removeLast();

            break;
        }
        index++;
    }


    doc.setArray(highscore);
    file.open(QIODevice::WriteOnly);
    file.write(doc.toBinaryData());
    file.close();

    return true;
}

/**
 * @brief Model::getHighScores : reads the highscores.dat to extract the data
 * @return : QVector<Qhighscores>
 */
const QVector<Qhighscore> Model::getHighScores()
{
    QVector<Qhighscore> highScoresVect;

    QFile file("../DonkeyKongv2/res/highscore.dat");
    if (!file.open(QIODevice::ReadOnly))
        return highScoresVect;

    QJsonDocument doc = QJsonDocument::fromBinaryData(file.readAll());
    if (doc.isNull())
        return highScoresVect;

    file.close();

    QJsonArray highscore = doc.array();
    foreach (const QJsonValue &val, highscore) {

        QJsonObject player = val.toObject();
        Qhighscore jepasdidedenom;
        jepasdidedenom.playerName = player["Name"].toString();
        jepasdidedenom.score = player["Score"].toInt();

        highScoresVect.append(jepasdidedenom);
    }
    return highScoresVect;
}
