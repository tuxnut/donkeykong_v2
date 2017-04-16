#include "visitor.h"

Visitor::Visitor(Model *model)
{
    this->model = model;
}

void Visitor::savingVisit(Player *p)
{
    QJsonObject obj;
    obj["Score"] = p->getScore();
    obj["NbBananas"] = p->getNbBananas();
    obj["NbBlocksDestroyed"] = p->getNbBlockDestroyed();

    QJsonDocument doc(obj);

    QFile file("saver.dat");
    if (!file.open(QIODevice::WriteOnly)) {
        qWarning("couldnn't create saving file");
        return;
    }

    file.write(doc.toBinaryData());
    file.close();

    return;
}
