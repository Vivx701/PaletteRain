#include <Paletterain.h>
#include <QHBoxLayout>
#include <QMessageBox>

PaletteRain::PaletteRain(QWidget *parent)
    : QMainWindow(parent)
{

    setWindowTitle("Palette Rain");
    centralWidget = new QWidget(this);
    game = new RainWidget(this);
    status = new StatusWidget(this);
    status->setFixedSize(200, 200);


    QHBoxLayout *layout = new QHBoxLayout();
    layout->addWidget(game, 3);
    layout->addWidget(status, 1, Qt::AlignCenter);

    QWidget *central = new QWidget();
    central->setStyleSheet("background-color: #000000;");
    central->setLayout(layout);
    setCentralWidget(central);

    levelUpSound = new QSoundEffect(this);
    gameOverSound   = new QSoundEffect(this);
    levelUpSound->setVolume(1.0);
    gameOverSound->setVolume(1.0);
    levelUpSound->setSource(QUrl::fromLocalFile("://Music/levelup.wav"));
    gameOverSound->setSource(QUrl::fromLocalFile("://Music/gameover.wav"));


    game->setFocusPolicy(Qt::StrongFocus);
    game->setFocus();

    connect(game, SIGNAL(scoreChanged(int)), status, SLOT(setScore(int)));
    connect(game, SIGNAL(levelChanged(int)), status, SLOT(setLevel(int)));    
    connect(game, &RainWidget::gameOver, this, &PaletteRain::showGameOverDialog);
    connect(game, &RainWidget::helpRequested, this, &PaletteRain::showHelpDialog);
    //playing sound effect
    connect(game, &RainWidget::levelChanged, [this](int){
        playSoundEffect(levelUpSound);
    });
    setFixedSize(centralWidget->sizeHint());
}


void PaletteRain::showGameOverDialog(int score, int level) {
     playSoundEffect(gameOverSound);
    QString message = QString("💀 GAME OVER 💀\n\n")
                      + "Final Score: " + QString::number(score) + "\n"
                      + "Level: " + QString::number(level) + "\n\n"
                      + "Play again?";

    QMessageBox msgBox;
    msgBox.setWindowTitle("Game Over");
    msgBox.setText(message);
    msgBox.setIcon(QMessageBox::Information);
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::Yes);

    // Retro style using style sheet
    msgBox.setStyleSheet(
        "QMessageBox { background-color: #111; color: #00FF00; font-family: Courier; font-size: 14px; } "
        "QLabel { color: white; font: bold 20px 'Courier'; }"
        "QPushButton { background-color: #222; color: #FFD700; border: 2px solid #FF4500; padding: 4px; } "
        "QPushButton:hover { background-color: #333; }"
        );

    int ret = msgBox.exec();
    if (ret == QMessageBox::Yes) {
        game->resetGame();
    } else {
        // Optional: Close app or do nothing
        close(); // if RainWidget is the main window
    }
}

void PaletteRain::showHelpDialog()
{
    QString helpText = R"(

🕹️ Palette Rain - How to Play 🕹️

Match 3 or more blocks of the same color either horizontally or vertically.
Blocks fall in sets of 3. Rotate their colors, move them, and match them to score points.
As your score increases, the speed increases. The game ends if blocks reach the top.

Controls:

  Key       | Action
  ----------|--------------------------
  ⬅️ Left   | Move droplet left
  ➡️ Right  | Move droplet right
  ⬇️ Down   | Speed up fall
  ⬆️ Up     | Rotate droplet colors
  F1        | Show help screen

🎯 Score increases by 10 per block cleared.
🚀 Level increases every 500 points, and speed increases with level.

👨‍💻 **Developer**
Vivek P
🌐 [GitHub](https://github.com/Vivx701)
✉️ vivx_developer@yahoo.in
🔗 [LinkedIn](https://linkedin.com/in/vivek-p-87323b111)

)";

    helpText += "Built with Qt version:" QT_VERSION_STR;
    QMessageBox *msg = new QMessageBox(this);
    msg->setWindowTitle("How to Play - Palette Rain");
    msg->setText(helpText);

    // Apply retro style
    msg->setStyleSheet(R"(
    QMessageBox {
        background-color: black;
        color: white;
        font-family: 'Courier';
        font-size: 11pt;
    }
    QLabel {
        color: white;
    }
    QPushButton {
        background-color: #222;
        color: white;
        padding: 6px;
        border: 1px solid #555;
    }
    QPushButton:hover {
        background-color: #444;
    }
)");

    msg->setStandardButtons(QMessageBox::Ok|QMessageBox::Help);
    game->pauseGame();
    int ret = msg->exec();
    if (ret == QMessageBox::Help) {
        QMessageBox::aboutQt(this);
    }
    game->resumeGame();

}

void PaletteRain::playSoundEffect(QSoundEffect *effect)
{
    if(effect == nullptr)
    {
        return;
    }
    if(effect->isPlaying())
    {
        effect->stop();
    }
    effect->play();
}

PaletteRain::~PaletteRain()
{

}
