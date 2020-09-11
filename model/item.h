#ifndef ITEM_H
#define ITEM_H

#include <QImage>
#include <QObject>
#include <QString>
#include <QVector2D>

namespace parkour {

class Item : public QObject {
    Q_OBJECT
public:
    virtual QString getDisplayName() = 0;
    virtual const QImage& getIcon() = 0;
    virtual void onUse(QVector2D) = 0;
};

}

#endif