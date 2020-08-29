#include "blocktnt.h"
#include "../controller/worldcontroller.h"
#include "entitytnt.h"
#include "world.h"

namespace parkour {
BlockTNT::BlockTNT() {
}

QString BlockTNT::getName() const {
    return "tnt";
}

QString BlockTNT::getResourceLocation() const {
    return ":/assets/blocks/tnt.png";
}

double BlockTNT::getExplosionResistance() const {
    return 1;
}
}

void parkour::BlockTNT::onExplosion(QPoint blockPos, double power) {
    Q_UNUSED(power);
    WorldController::instance().explode(blockPos, TNT_EXPLOSION_POWER);
}

void parkour::BlockTNT::collide(QPoint blockPos, const Entity& entity, Direction dir) {
    Q_UNUSED(entity)
    Q_UNUSED(dir)
    auto& world = World::instance();
    world.setBlock(blockPos, "air");
    auto entityTnt = QSharedPointer<EntityTNT>::create();
    entityTnt->placeBoundingBoxAt(QVector2D(blockPos));
    world.addEntity(entityTnt);
}