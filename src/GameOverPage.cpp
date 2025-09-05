#include "GameOverPage.h"
#include <QVBoxLayout>

GameOverPage::GameOverPage(QWidget *parent)
    : QWidget{parent}
{
    setObjectName("Gameover");
    titleLabel = new QLabel("GAME OVER", this);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setObjectName("tittle");
    messageLabel = new QLabel(this);
    messageLabel->setAlignment(Qt::AlignCenter);
    messageLabel->setObjectName("msg");
    this->setLayout(new QVBoxLayout());
    this->layout()->addWidget(titleLabel);
    this->layout()->addWidget(messageLabel);
    this->layout()->setSpacing(0);
    this->layout()->setSizeConstraint(QLayout::SetNoConstraint);
}

void GameOverPage::setScoreAndLevel(int score, int level)
{
    // Central label text
    messageLabel->setText(
        tr("Final Score: %1\nLevel: %2\n Press R to restart the game")
            .arg(score)
            .arg(level)
        );
}
