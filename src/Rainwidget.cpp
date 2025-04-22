#include "Rainwidget.h"
#include <QPainter>
#include <QKeyEvent>
#include <QRandomGenerator>
#include <QThread>

RainWidget::RainWidget(QWidget *parent) : QWidget(parent) {

    grid.resize(rows);
    for (int i = 0; i < rows; ++i)
        grid[i].resize(cols, nullptr);

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &RainWidget::updateGame);
    timer->start(400); // drop speed
    setMinimumSize(QSize(cols * tileSize+10, rows * tileSize+10));
    spawnDroplet();
}

void RainWidget::resizeEvent(QResizeEvent *event) {
    QWidget::resizeEvent(event);

    // Dynamically calculate tileSize based on new widget size
    int newTileWidth = width() / cols;
    int newTileHeight = height() / rows;
    tileSize = qMin(newTileWidth, newTileHeight);
    update(); // Repaint
}

QSize RainWidget::sizeHint() const {
    return QSize(cols * tileSize, rows * tileSize);
}

void RainWidget::paintEvent(QPaintEvent *) {
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);

    const int borderMargin = 2; // Match border size and spacing

    // 🟦 Draw retro border
    QPen borderPen(QColor("#00FFFF"));  // neon cyan
    borderPen.setWidth(borderMargin);   // thickness of the border
    p.setPen(borderPen);
    p.setBrush(Qt::NoBrush);
    p.drawRect(rect().adjusted(borderMargin / 2,
                               borderMargin / 2,
                               -borderMargin / 2,
                               -borderMargin / 2));

    // 🔲 Draw grid blocks inside the border
    int offsetX = borderMargin+3;
    int offsetY = borderMargin+3;

    for (int y = 0; y < rows; ++y) {
        for (int x = 0; x < cols; ++x) {
            if (grid[y][x]) {
                p.setPen(Qt::white);
                p.setBrush(grid[y][x]->color);
                p.drawRect(offsetX + x * tileSize,
                           offsetY + y * tileSize,
                           tileSize, tileSize);
            }
        }
    }

    // 🎮 Draw falling blocks
    for (int i = 0; i < fallingBlocks.size(); ++i) {
        const QPoint &pos = fallingPositions[i];
        if (pos.y() >= 0) {
            p.setBrush(fallingBlocks[i]->color);
            p.setPen(Qt::white);
            p.drawRect(offsetX + pos.x() * tileSize,
                       offsetY + pos.y() * tileSize,
                       tileSize, tileSize);
        }
    }
}



void RainWidget::keyPressEvent(QKeyEvent *event) {
    if (fallingBlocks.isEmpty())
        return;

    switch (event->key()) {
    case Qt::Key_Left:
        moveDroplet(-1, 0);
        break;
    case Qt::Key_Right:
        moveDroplet(1, 0);
        break;
    case Qt::Key_Down:
        moveDroplet(0, 1);
        break;
    case Qt::Key_Up:
        if (fallingBlocks.size() >= 2) {
            // Rotate the colors upward (last becomes first)
            QColor lastColor = fallingBlocks.last()->color;
            for (int i = fallingBlocks.size() - 1; i > 0; --i) {
                fallingBlocks[i]->color = fallingBlocks[i - 1]->color;
            }
            fallingBlocks[0]->color = lastColor;
        }
        break;
    case Qt::Key_F1:
        emit helpRequested();
        break;
    }

    update();
}


void RainWidget::updateGame() {
    if (!moveDroplet(0, 1)) {
        lockDroplet();
        checkAndClearAllMatches();
        spawnDroplet();
    }
    update();
}

bool RainWidget::getPaused() const
{
    return paused;
}


void RainWidget::spawnDroplet() {

    fallingBlocks.clear();
    fallingPositions.clear();

    for (int i = 0; i < 3; ++i) {
        fallingBlocks.append(new Droplet());
        fallingPositions.append(QPoint(cols / 2, -i));  // Stacked vertically above visible grid
    }

    // Check if any position already occupied
    for (const QPoint &pos : fallingPositions) {
        if (pos.y() >= 0 && grid[pos.y()][pos.x()]) {
            timer->stop();
            isGameOver = true;
            emit gameOver(score, level);
            return;
        }
    }
}


bool RainWidget::moveDroplet(int dx, int dy) {
    QVector<QPoint> newPositions;
    for (const QPoint &pos : fallingPositions) {
        QPoint newPos = pos + QPoint(dx, dy);
        if (newPos.x() < 0 || newPos.x() >= cols || newPos.y() >= rows)
            return false;
        if (newPos.y() >= 0 && grid[newPos.y()][newPos.x()])
            return false;
        newPositions.append(newPos);
    }

    fallingPositions = newPositions;
    return true;
}



void RainWidget::lockDroplet() {
    for (int i = 0; i < fallingBlocks.size(); ++i) {
        QPoint pos = fallingPositions[i];
        if (pos.y() >= 0) {
            grid[pos.y()][pos.x()] = fallingBlocks[i];
        } else {
            delete fallingBlocks[i];  // Block was outside the visible area
        }
    }

    fallingBlocks.clear();
    fallingPositions.clear();
}

void RainWidget::clearMatches() {
    QSet<QPoint> toClear;  // ✅ Use QSet to avoid duplicates
    clearedBlocks = 0;

    // 🔁 Horizontal check
    for (int y = 0; y < rows; ++y) {
        int count = 1;
        for (int x = 1; x < cols; ++x) {
            if (grid[y][x] && grid[y][x - 1] &&
                grid[y][x]->color == grid[y][x - 1]->color) {
                count++;
            } else {
                if (count >= 3) {
                    for (int i = 0; i < count; ++i)
                        toClear.insert(QPoint(x - i - 1, y));
                }
                count = 1;
            }
        }
        if (count >= 3) {
            for (int i = 0; i < count; ++i)
                toClear.insert(QPoint(cols - i - 1, y));
        }
    }

    // 🔁 Vertical check
    for (int x = 0; x < cols; ++x) {
        int count = 1;
        for (int y = 1; y < rows; ++y) {
            if (grid[y][x] && grid[y - 1][x] &&
                grid[y][x]->color == grid[y - 1][x]->color) {
                count++;
            } else {
                if (count >= 3) {
                    for (int i = 0; i < count; ++i)
                        toClear.insert(QPoint(x, y - i - 1));
                }
                count = 1;
            }
        }
        if (count >= 3) {
            for (int i = 0; i < count; ++i)
                toClear.insert(QPoint(x, rows - i - 1));
        }
    }

    // 💣 Clear the matched blocks
    clearedBlocks = toClear.size();
    for (const QPoint &pt : toClear) {
        if (grid[pt.y()][pt.x()]) {
            delete grid[pt.y()][pt.x()];
            grid[pt.y()][pt.x()] = nullptr;
        }
    }

    // ⬇️ Drop blocks above
    for (int x = 0; x < cols; ++x) {
        for (int y = rows - 2; y >= 0; --y) {
            if (grid[y][x] && !grid[y + 1][x]) {
                int dropY = y;
                while (dropY + 1 < rows && !grid[dropY + 1][x]) {
                    std::swap(grid[dropY][x], grid[dropY + 1][x]);
                    dropY++;
                }
            }
        }
    }

    // 🧮 Scoring and leveling
    score += (clearedBlocks * 10);
    emit scoreChanged(score);

    if (score >= nextLevelScore) {
        level++;
        nextLevelScore += 500;
        int newInterval = qMax(100, 400 - (level - 1) * 25);
        timer->setInterval(newInterval);
        emit levelChanged(level);
    }
}

void RainWidget::checkAndClearAllMatches()
{
    do {
        clearMatches();
    } while (clearedBlocks > 0);
}


void RainWidget::resetGame() {
    // Clear the grid
    for (int y = 0; y < rows; ++y) {
        for (int x = 0; x < cols; ++x) {
            delete grid[y][x];
            grid[y][x] = nullptr;
        }
    }

    // Clear falling blocks and positions
    for (Droplet* d : fallingBlocks) {
        delete d;
    }
    fallingBlocks.clear();
    fallingPositions.clear();

    // Restart the game timer
    if (!timer->isActive()) {
        timer->start(400); // reset to initial speed
    }
    score = 0;
    level = 1;
    emit levelChanged(level);
    emit scoreChanged(score);
    // Spawn the first droplet
    spawnDroplet();

    // Redraw the grid
    update();
}

void RainWidget::pauseGame() {
    if (!paused) {
        timer->stop();
        paused = true;
    }
}

void RainWidget::resumeGame() {
    if (paused) {
        timer->start();  // resumes with last interval
        paused = false;
    }
}
