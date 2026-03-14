#include <Paletterain.h>
#include <GameVersion.h>
#include <QApplication>
#include <QFile>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QCoreApplication::setApplicationName(GAME_TITTLE);
    QCoreApplication::setApplicationVersion(GAME_VERSION_STRING);
    // Optional: print version info
    qDebug() << "Running " << QCoreApplication::applicationName()<<"  "<<QCoreApplication::applicationVersion();
    qDebug() << "Build Date:" << GAME_BUILD_DATE << "Time:" << GAME_BUILD_TIME;
    // Load stylesheet from resources
    QFile file(":/style.qss");
    if (file.open(QFile::ReadOnly | QFile::Text)) {
        QTextStream stream(&file);
        QString styleSheet = stream.readAll();
        a.setStyleSheet(styleSheet);
        file.close();
    }
    PaletteRain w;
#ifdef Q_OS_WIN
    w.setWindowFlags(w.windowFlags() & ~Qt::WindowMaximizeButtonHint);
#endif
    w.show();
    qDebug() << "Size "<<w.sizeHint();
    return a.exec();
}
