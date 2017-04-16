#include "model.h"

#include <QDebug>

Model::Model()
{

}

bool Model::saveGameAuto(Player * p)
{
    QJsonObject obj;
    obj["Score"] = p->getScore();
    obj["NbBananas"] = p->getNbBananas();
    obj["NbBlocksDestroyed"] = p->getNbBlockDestroyed();

    QJsonDocument doc(obj);

    QFile file("saver.dat");
    if (!file.open(QIODevice::WriteOnly)) {
        qWarning("couldnn't create saving file");
        return false;
    }

    file.write(doc.toBinaryData());
    file.close();

    return true;
}

bool Model::loadPlayer(const QString &dir, Player *p)
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
