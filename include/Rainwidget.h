#ifndef RAINWIDGET_H
#define RAINWIDGET_H

#include <QWidget>
#include <QTimer>
#include <QVector>
#include "Droplet.h"

class RainWidget : public QWidget {
    Q_OBJECT

public:
    RainWidget(QWidget *parent = nullptr);
    QSize sizeHint() const override;
    void resetGame();
    void pauseGame();
    void resumeGame();
    bool getPaused() const;

protected:
    void paintEvent(QPaintEvent *) override;
    void keyPressEvent(QKeyEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private slots:
    void updateGame();

private:
    const int rows = 20;
    const int cols = 10;
    int tileSize = 32;
    bool isGameOver = false;
    bool paused = false;

    QVector<QVector<Droplet*>> grid;
    QVector<Droplet*> fallingBlocks;
    QVector<QPoint> fallingPositions;
    QTimer *timer;
    int score = 0;
    int level = 1;
    int clearedBlocks = 0;
    int nextLevelScore = 500;

    void spawnDroplet();
    bool moveDroplet(int dx, int dy);
    void lockDroplet();
    void clearMatches();
    void showGameOverDialog();
    void checkAndClearAllMatches();

signals:
    void scoreChanged(int score);
    void levelChanged(int level);
    void gameOver(int finalScore, int level);
    void helpRequested();
};

#endif // RAINWIDGET_H
