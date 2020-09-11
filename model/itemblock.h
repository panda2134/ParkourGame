#pragma once

#include "block.h"
#include "item.h"
#include <QSharedPointer>

namespace parkour {
class ItemBlock : public Item {
    Q_OBJECT

    QSharedPointer<Block> block;
    QImage texture;

public:
    ItemBlock(QSharedPointer<Block>);

    virtual QString getDisplayName() override;
    virtual const QImage& getIcon() override;
    virtual void onUse(QVector2D) override;
};

}