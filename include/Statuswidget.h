#ifndef STATUSWIDGET_H
#define STATUSWIDGET_H


#include <QWidget>

class StatusWidget : public QWidget {
    Q_OBJECT

public:
    explicit StatusWidget(QWidget *parent = nullptr);

public slots:
    void setScore(int score);
    void setLevel(int level);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    int score;
    int level;
    QString smiley;
    int fontSize;
    void updateSmiley(int prevScore, int newScore);
};

#endif // STATUSWIDGET_H
