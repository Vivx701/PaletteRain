#ifndef PALETTERAIN_H
#define PALETTERAIN_H

#include <Rainwidget.h>
#include <Statuswidget.h>
#include <QMainWindow>
#include <QSoundEffect>

class PaletteRain : public QMainWindow {
    Q_OBJECT

private:
    QWidget *centralWidget;
    RainWidget *game;
    StatusWidget *status;
    QSoundEffect *levelUpSound;
    QSoundEffect *gameOverSound;
public:
    PaletteRain(QWidget *parent = nullptr);
    ~PaletteRain();
    void showGameOverDialog(int score, int level);
    void playSoundEffect(QSoundEffect *effect);
};

#endif // PALETTERAIN_H
