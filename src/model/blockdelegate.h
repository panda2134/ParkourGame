#ifndef BLOCKDELEGATE_H
#define BLOCKDELEGATE_H

#include "block.h"
#include "entity.h"
#include "icollidable.h"
#include "registry.h"
#include <QObject>
#include <QString>
#include <QVector2D>

namespace parkour {

class BlockDelegate : public QObject, public ICollidable {
    Q_OBJECT
    QSharedPointer<Block> block;
    QPoint blockPos;

public:
    /**
     * @brief BlockDelegate
     * @param blockName_ 方块名称
     * @param blockPos_ 方块位置
     */
    BlockDelegate(QString blockName_, QPoint blockPos_);

    BoundingBoxWorld getBoundingBoxWorld() const override;
    void collide(const ICollidable&, Direction) override;
    QVector2D getVelocity() const override;

    bool isAir() { return block == nullptr; }
};

}

#endif // BLOCKDELEGATE_H
