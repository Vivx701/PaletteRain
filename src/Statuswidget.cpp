#include "Statuswidget.h"
#include <QPainter>
#include <QFont>
#include <QTimer>

StatusWidget::StatusWidget(QWidget *parent)
    : QWidget(parent), score(0), level(1)
{
    fontSize = qMax(20, height() / 10);
    helpMsg = "F1(Help)";
    smileys = {
                QImage(":/Images/smile.png"),
                QImage(":/Images/fun.png"),
                QImage(":/Images/wow.png"),
                QImage(":/Images/love.png"),
                QImage(":/Images/wait.png")
              };
    currentSmileyIndex = 0;
}

void StatusWidget::setScore(int newScore) {

    int diff = (newScore - score);
    if(diff == 30)
    {
        currentSmileyIndex = 1;
    }
    else if (diff >= 40)
    {
        currentSmileyIndex = 2;
    }
    if(diff >= 30)
    {
        QTimer::singleShot(3000, this, [this](){currentSmileyIndex=0;update();});
    }
    score = newScore;
    update(); // Trigger repaint

}

void StatusWidget::setLevel(int newLevel) {
    level = newLevel;
    currentSmileyIndex = 3;
    update(); // Trigger repaint
    QTimer::singleShot(3000, this, [this](){currentSmileyIndex=0;update();});
}

void StatusWidget::setPaused(bool paused)
{
    pauseMsg = paused ? "Paused": "";
    currentSmileyIndex = paused ? 4: 0;
    update();
}

void StatusWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::SmoothPixmapTransform, true);
    painter.fillRect(rect(), Qt::black);

    // 🔥 Fire gradient for text
    QLinearGradient fireGradient(0, 0, 0, 100);
    fireGradient.setColorAt(0.0, QColor("#FFD700"));  // Gold
    fireGradient.setColorAt(0.5, QColor("#FF8C00"));  // Dark Orange
    fireGradient.setColorAt(1.0, QColor("#FF4500"));  // Orange Red

    painter.setPen(QPen(QBrush(fireGradient), 0));
    QFont font("Courier", fontSize, QFont::Bold);
    painter.setFont(font);

    // Text
    painter.drawText(10, 20, QString("SCORE: %1").arg(score));
    painter.drawText(10, 50, QString("LEVEL: %1").arg(level));
    painter.drawText(10, 95, helpMsg);
    //Paused
    painter.drawText(10, 125, pauseMsg);

    //draw smiley
    painter.drawImage(QRect(10, 150, 100, 100), smileys[currentSmileyIndex]);

}
