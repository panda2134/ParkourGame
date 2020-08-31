#ifndef BLOCKSPRING_H
#define BLOCKSPRING_H

#include "block.h"
#include <QHash>
#include <QObject>
#include <QPair>

namespace parkour {
class BlockSpring : public Block
{
    Q_OBJECT

    const int FRAME_COUNT = 4;
    const int currentFrame = 0;
    const double BOUNCE_VELOCITY = -35.0;

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
    void collide(QPoint, Entity&, Direction) override;
};
}

#endif // BLOCKSPRING_H
