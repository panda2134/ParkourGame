#ifndef BLOCKSPRING_H
#define BLOCKSPRING_H

#include "block.h"
#include <QHash>
#include <QObject>
#include <QPair>

namespace parkour {
class BlockSpring : public Block {
    Q_OBJECT

    const int FRAME_COUNT = 4;
    const int currentFrame = 0;
    const double BOUNCE_VELOCITY = -30.0;

    QHash<QPair<int, int>, int> frameCount;

public:
    BlockSpring();

    // Block interface
public:
    QString getName() const override;
    QString getResourceLocation() const override;
    double getExplosionResistance() const override;
    int getCurrentFrame(const QPoint& blockPos) override;
    void onStand(QPoint blockPos, Entity& entity) override;
    BoundingBox getBoundingBox() const override;
    virtual QString getDisplayName() const override;
    void collide(QPoint, Entity&, Direction) override;
    bool canPlaceAt(QPoint blockPos) const;
    bool isSolid() const;
};
}

#endif // BLOCKSPRING_H
