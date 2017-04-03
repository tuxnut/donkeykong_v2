#include <QApplication>

#include "view.h"
#include "model.h"
#include "coregame.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    View w;
    Model m;
    CoreGame(&m, w);
    w.show();

    return a.exec();
}
