#ifndef GAMEOVERPAGE_H
#define GAMEOVERPAGE_H

#include <QWidget>
#include <QLabel>

class GameOverPage : public QWidget
{
    Q_OBJECT
public:
    explicit GameOverPage(QWidget *parent = nullptr);

public slots:
    void setScoreAndLevel(int score, int level);
private:
    QLabel* titleLabel;
    QLabel* messageLabel;

signals:
};

#endif // GAMEOVERPAGE_H
