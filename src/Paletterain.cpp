#include <Paletterain.h>
#include <QHBoxLayout>
#include <QLabel>
#include <QKeyEvent>

PaletteRain::PaletteRain(QWidget *parent)
    : QMainWindow(parent)
{

    setWindowTitle("Palette Rain");
    centralWidget = new QStackedWidget(this);
    setCentralWidget(centralWidget);
    setFixedSize(centralWidget->sizeHint());
    createPages();
}

void PaletteRain::showGameOver(int score, int level)
{
    playSoundEffect(gameOverSound);
    gameOverPage->setScoreAndLevel(score, level);
    centralWidget->setCurrentIndex(GAMEOVERPAGEINDEX);
    gameOverPage->setFocus(Qt::ActiveWindowFocusReason);
}

void PaletteRain::showHelpDialog()
{
    if(centralWidget->currentIndex() == GAMEPAGEINDEX)
    {
        centralWidget->setCurrentIndex(HELPPAGEINDEX);
        game->pauseGame();
    }
    else
    {
        centralWidget->setCurrentIndex(GAMEPAGEINDEX);
        game->resumeGame();
    }
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

bool PaletteRain::eventFilter(QObject *watched, QEvent *event)
{
    if (event->type() == QEvent::KeyPress) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
        switch (keyEvent->key()) {
        case Qt::Key_F1:
             {
                if(watched == centralWidget->widget(HELPPAGEINDEX))
                {
                    showHelpDialog();
                    return true;
                }
             }
            break;
        case Qt::Key_R:
            {
                if(watched == centralWidget->widget(GAMEOVERPAGEINDEX))
                {
                    centralWidget->setCurrentIndex(GAMEPAGEINDEX);
                    QCoreApplication::sendEvent(game, event);
                    return true;
                }
            }
        break;
        default:
            break;
        }
    }

    // Default: pass the event on
    return QObject::eventFilter(watched, event);
}

void PaletteRain::createPages()
{
    centralWidget->addWidget(createGamePage());
    centralWidget->addWidget(createHelpPage());
    gameOverPage = new GameOverPage();
    gameOverPage->installEventFilter(this);
    centralWidget->addWidget(gameOverPage);
}

QFrame *PaletteRain::createGamePage()
{
    QFrame *gamePage = new QFrame();
    gamePage->setStyleSheet("background-color: #000000;");

    game = new RainWidget(this);
    status = new StatusWidget(this);
    status->setFixedSize(200, 400);

    levelUpSound   = new QSoundEffect(this);
    gameOverSound  = new QSoundEffect(this);
    gamePauseSound = new QSoundEffect(this);
    levelUpSound->setSource(QUrl::fromLocalFile("://Music/levelup.wav"));
    gameOverSound->setSource(QUrl::fromLocalFile("://Music/gameover.wav"));
    gamePauseSound->setSource(QUrl::fromLocalFile("://Music/pause.wav"));
    levelUpSound->setVolume(1.0);
    gameOverSound->setVolume(1.0);
    gamePauseSound->setVolume(1.0);

    game->setFocusPolicy(Qt::StrongFocus);
    game->setFocus();

    QHBoxLayout *layout = new QHBoxLayout();
    layout->addWidget(game, 3);
    layout->addWidget(status, 1, Qt::AlignCenter);
    gamePage->setLayout(layout);

    connect(game, SIGNAL(scoreChanged(int)), status, SLOT(setScore(int)));
    connect(game, SIGNAL(levelChanged(int)), status, SLOT(setLevel(int)));
    connect(game, SIGNAL(gamePaused(bool)), status, SLOT(setPaused(bool)));
    connect(game, &RainWidget::gameOver, this, &PaletteRain::showGameOver);
    connect(game, &RainWidget::helpRequested, this, &PaletteRain::showHelpDialog);
    //playing sound effect
    connect(game, &RainWidget::levelChanged, [this](int){
        playSoundEffect(levelUpSound);
    });
    connect(game,  &RainWidget::gamePaused, [this](bool) {
        playSoundEffect(gamePauseSound);
    });
    return gamePage;
}

QFrame *PaletteRain::createHelpPage()
{
    QFrame *msgPage = new QFrame(this);
    const QString helpHtml = R"(<!DOCTYPE html>
  <html>
  <body style="margin:0; font-family:'Segoe UI','DejaVu Sans',Arial,sans-serif; font-size:12pt; color:#ffffff;">
    <!-- Header -->
    <div style="text-align:center; padding:10px; border-bottom:2px solid #444; margin-bottom:10px;">
      <h2 style="margin:0; font-size:22pt; font-weight:800; letter-spacing:.5px;">
        How to Play – PaletteRain
      </h2>
    </div>

    <!-- Main Content -->
    <p style="margin:8px 0 0 0;">
      Match <b>3 or more</b> blocks of the same color either horizontally or vertically.<br/>
      Blocks fall in sets of 3. Rotate their colors, move them, and match them to score points.<br/>
      As your score increases, the speed increases. The game ends if blocks reach the top.
    </p>

    <h3 style="margin:16px 0 6px 0;">Controls</h3>
    <table style="border-collapse:collapse; margin:4px 0 10px 0;">
      <tr>
        <th style="text-align:left; padding:4px 10px 4px 0; border-bottom:1px solid #666;">Key</th>
        <th style="text-align:left; padding:4px 0; border-bottom:1px solid #666;">Action</th>
      </tr>
      <tr><td style="padding:6px 10px 6px 0;">⬅️ Left</td>  <td style="padding:6px 0;">Move droplet left</td></tr>
      <tr><td style="padding:6px 10px 6px 0;">➡️ Right</td> <td style="padding:6px 0;">Move droplet right</td></tr>
      <tr><td style="padding:6px 10px 6px 0;">⬇️ Down</td>  <td style="padding:6px 0;">Speed up fall</td></tr>
      <tr><td style="padding:6px 10px 6px 0;">⬆️ Up</td>    <td style="padding:6px 0;">Rotate droplet colors</td></tr>
      <tr><td style="padding:6px 10px 6px 0;">F1</td>       <td style="padding:6px 0;">Show/Hide help screen</td></tr>
      <tr><td style="padding:6px 10px 6px 0;">ESC</td>      <td style="padding:6px 0;">Pause/Resume game</td></tr>
    </table>

    <p style="margin:8px 0;">
      🎯 <b>Score</b> increases by <b>10</b> per block cleared.<br/>
      🚀 <b>Level</b> increases every <b>500</b> points, and speed increases with level.
    </p>

    <h3 style="margin:16px 0 6px 0;">👨‍💻 Developer</h3>
    <p style="margin:4px 0;">
      <b>Vivek P</b><br/>
      🌐 <a href="https://github.com/Vivx701">GitHub</a><br/>
      🔗 <a href="https://linkedin.com/in/vivek-p-87323b111">LinkedIn</a>
    </p>
  </body>
</html>)";

    // Apply retro style
    msgPage->setStyleSheet(R"(
    QWidget {
        background-color: #000;   /* Full black background */
    }
    QLabel {
        color: #ddd;              /* Light gray text */
        font-size: 12pt;
    }
    QLabel[role="header"] {
        color: #00e0ff;           /* Cyan title text */
        font-weight: bold;
        font-size: 16pt;
    }
    QLabel[role="footer"] {
        color: #888;              /* Dim gray footer text */
        font-size: 9pt;
    }
    QPushButton {
        background-color: #222;
        color: #fff;
        padding: 6px 12px;
        border: 1px solid #555;
        border-radius: 4px;
    }
    QPushButton:hover {
        background-color: #333;
        border-color: #777;
    }
    QPushButton:pressed {
        background-color: #111;
        border-color: #999;
    }
)");

    QLabel* helpLabel = new QLabel(msgPage);
    helpLabel->setTextFormat(Qt::RichText);
    helpLabel->setWordWrap(true);
    helpLabel->setOpenExternalLinks(true);
    helpLabel->setText(helpHtml);

    msgPage->setLayout(new QVBoxLayout());
    msgPage->layout()->addWidget(helpLabel);

    QLabel* qtVersionLabel = new QLabel(msgPage);
    qtVersionLabel->setText("Built with Qt version : " QT_VERSION_STR );
    qtVersionLabel->setAlignment(Qt::AlignHCenter);
    msgPage->layout()->addWidget(qtVersionLabel);
    msgPage->installEventFilter(this);
    return msgPage;
}

PaletteRain::~PaletteRain()
{
}
