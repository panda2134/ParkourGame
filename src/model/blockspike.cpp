#include "blockspike.h"
#include "registry.h"
#include "world.h"

namespace parkour {
BlockSpike::BlockSpike() {
}

QString BlockSpike::getName() const {
    return "spike";
}

QString BlockSpike::getResourceLocation() const {
    return ":/assets/blocks/spike.png";
}

double BlockSpike::getExplosionResistance() const {
    return 12.0;
}

void BlockSpike::onStand(QPoint blockPos, Entity& entity) {
    Q_UNUSED(blockPos)
    entity.setVelocity(entity.getVelocity() + QVector2D(0, BOUNCE_VELOCITY));
    entity.damage(SPIKE_DAMAGE_PER_TICK);
}

QString BlockSpike::getDisplayName() const {
	return "尖刺方块";
}

void BlockSpike::collide(QPoint blockPos, Entity& entity, Direction dir) {
    if (dir == Direction::UP) {
        onStand(blockPos, entity);
    }
}

bool BlockSpike::canPlaceAt(QPoint blockPos) const {
	auto block = registry::BlockRegistry::instance().getBlockByName(World::instance().getBlock(blockPos + QPoint(0, 1)));
	return block != nullptr && block->isSolid();
}

bool BlockSpike::isSolid() const {
	return false;
}
}
