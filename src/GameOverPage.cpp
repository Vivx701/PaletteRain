#include "GameOverPage.h"
#include <QVBoxLayout>

GameOverPage::GameOverPage(QWidget *parent)
    : QWidget{parent}
{
    titleLabel = new QLabel("GAME OVER", this);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setObjectName("tittle");
    messageLabel = new QLabel(this);
    messageLabel->setAlignment(Qt::AlignCenter);
    this->setLayout(new QVBoxLayout());
    this->layout()->addWidget(titleLabel);
    this->layout()->addWidget(messageLabel);
    this->layout()->setSpacing(0);
    this->layout()->setSizeConstraint(QLayout::SetNoConstraint);

    setStyleSheet(R"(
        QWidget{
            background-color: #020618;
        }
        QLabel#tittle {
            color: #DFF2FE;
            font-weight: 800;
            font-size: 50px;
        }
        QLabel {
            color: #DFF2FE;
            font-weight: 800;
            font-size: 24px;
        }
        )");
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
