#ifndef PALETTERAIN_H
#define PALETTERAIN_H

#include <Rainwidget.h>
#include <Statuswidget.h>
#include <GameOverPage.h>
#include <QMainWindow>
#include <QSoundEffect>
#include <QStackedWidget>

constexpr int GAMEPAGEINDEX     = 0;
constexpr int HELPPAGEINDEX     = 1;
constexpr int GAMEOVERPAGEINDEX = 2;

class PaletteRain : public QMainWindow {
    Q_OBJECT

private:
    QStackedWidget *centralWidget;
    RainWidget *game;
    StatusWidget *status;
    GameOverPage *gameOverPage;
    QSoundEffect *levelUpSound;
    QSoundEffect *gameOverSound;
    QSoundEffect *gamePauseSound;

    void createPages();
    QFrame* createGamePage();
    QFrame* createHelpPage();

public:
    PaletteRain(QWidget *parent = nullptr);
    ~PaletteRain();
    void showGameOver(int score, int level);
    void showHelpDialog();
    void playSoundEffect(QSoundEffect *effect);
    bool eventFilter(QObject *watched, QEvent *event) override;
};

#endif // PALETTERAIN_H
