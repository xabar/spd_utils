#include "spdcreator.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    spdCreator w;
    w.show();

    return a.exec();
}
