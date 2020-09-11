#include "blocksaw.h"
#include "entityplayerlike.h"
#include "registry.h"
#include "world.h"

namespace parkour {
BlockSaw::BlockSaw() { }
QString BlockSaw::getName() const {
    return "saw";
}
QString BlockSaw::getResourceLocation() const {
    return ":/assets/blocks/saw.png";
}
double BlockSaw::getExplosionResistance() const {
    return 10.0;
}
void BlockSaw::collide(QPoint blockPos, Entity& entity, Direction dir) {
    if (dir == Direction::DOWN)
        return;
    try {
        auto& entityPlayerLike = dynamic_cast<EntityPlayerLike&>(entity);
        entityPlayerLike.damage(DAMAGE_PER_COLLISION);
        switch (dir) {
        case Direction::LEFT:
            entityPlayerLike.setVelocity(QVector2D(-BOUNCE_AWAY_SPEED, entityPlayerLike.getVelocity().y()));
            break;
        case Direction::RIGHT:
            entityPlayerLike.setVelocity(QVector2D(+BOUNCE_AWAY_SPEED, entityPlayerLike.getVelocity().y()));
            break;
        case Direction::UP:
            entityPlayerLike.setVelocity(QVector2D(entityPlayerLike.getVelocity().x(), -BOUNCE_AWAY_SPEED));
            break;
        }
    } catch (std::bad_cast) {
        /* not player-like, do nothing */
    }
}
bool BlockSaw::canPlaceAt(QPoint blockPos) const {
    auto block = registry::BlockRegistry::instance().getBlockByName(World::instance().getBlock(blockPos + QPoint(0, 1)));
    return block != nullptr && block->isSolid();
}
bool BlockSaw::isSolid() const {
    return false;
}
QString BlockSaw::getDisplayName() const {
    return "锯片";
}
}