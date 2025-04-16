#include "Paletterain.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    PaletteRain w;
    w.show();
    return a.exec();
}
