#include <Paletterain.h>
#include <GameVersion.h>
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QCoreApplication::setApplicationName(GAME_TITTLE);
    QCoreApplication::setApplicationVersion(GAME_VERSION_STRING);


    // Optional: print version info
    qDebug() << "Running " << QCoreApplication::applicationName()<<"  "<<QCoreApplication::applicationVersion();
    qDebug() << "Build Date:" << GAME_BUILD_DATE << "Time:" << GAME_BUILD_TIME;
    PaletteRain w;
    w.show();
    return a.exec();
}
