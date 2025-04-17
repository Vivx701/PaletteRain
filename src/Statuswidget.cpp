#include "Statuswidget.h"
#include <QPainter>
#include <QFont>

StatusWidget::StatusWidget(QWidget *parent)
    : QWidget(parent), score(0), level(1), smiley("😃") {
    fontSize = qMax(20, height() / 10);
    helpMsg = "F1(Help)";
}

void StatusWidget::setScore(int newScore) {

    int diff = (newScore - score);
    if(diff == 30)
    {
        smiley = "😊";
    }
    else if (diff >=40)
    {
        smiley = "😎";
    }
    else
    {
        smiley = "😐";
    }
    score = newScore;
    update(); // Trigger repaint
}

void StatusWidget::setLevel(int newLevel) {
    level = newLevel;
    smiley = "🚀";
    update(); // Trigger repaint
}


void StatusWidget::paintEvent(QPaintEvent *) {

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
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
    painter.drawText(10, 30, QString("SCORE: %1").arg(score));
    painter.drawText(10, 55, QString("LEVEL: %1").arg(level));
    painter.drawText(10, 100, smiley);
    painter.drawText(10, 150, helpMsg);
}
