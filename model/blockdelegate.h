#ifndef BLOCKDELEGATE_H
#define BLOCKDELEGATE_H

#include "block.h"
#include "icollidable.h"
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
    BlockDelegate(QPoint blockPos_);

    BoundingBoxWorld getBoundingBoxWorld() const override;
    void collide(ICollidable&, Direction) override;
    QVector2D getVelocity() const override;
    void setVelocity(const QVector2D&) override;
    double getMass() const override final;
    bool isAir() { return block == nullptr; }
    virtual QString getName() const override;
    virtual QString getType() const override;
    virtual bool isNull() const;
};

}

#endif // BLOCKDELEGATE_H
