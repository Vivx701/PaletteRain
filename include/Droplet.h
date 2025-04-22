#ifndef DROPLET_H
#define DROPLET_H

#include <QColor>
#include <QRandomGenerator>

class Droplet {
public:
    QColor color;

    Droplet() {
        static QVector<QColor> palette = {
            QColor("#f44336"), QColor("#1E90FF"), QColor("#00FF00"), QColor("#ffee58"),
            QColor("#8A2BE2")
        };
        color = palette[QRandomGenerator::global()->bounded(palette.size())];
    }
};

#endif // DROPLET_H
