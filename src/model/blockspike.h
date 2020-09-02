#ifndef BLOCKSPIKE_H
#define BLOCKSPIKE_H

#include "../utils/consts.h"
#include "../utils/direction.h"
#include "block.h"

namespace parkour {
class BlockSpike : public Block
{
    Q_OBJECT

    const double SPIKE_DAMAGE_PER_TICK = 1.0 / TICKS_PER_SEC * 50.0;
    const double BOUNCE_VELOCITY = -5.0;

public:
    BlockSpike();

    // Block interface
public:
    QString getName() const override;
    QString getResourceLocation() const override;
    double getExplosionResistance() const override;
    void onStand(QPoint blockPos, Entity& entity) override;
	virtual QString getDisplayName() const override;
	void collide(QPoint, Entity&, Direction) override;
};
}

#endif // BLOCKSPIKE_H
