#include "blockspring.h"
#include "registry.h"
#include "world.h"

namespace parkour {
BlockSpring::BlockSpring() {
}

QString BlockSpring::getName() const {
    return "spring";
}

QString BlockSpring::getResourceLocation() const {
    return ":/assets/blocks/spring.png";
}

double BlockSpring::getExplosionResistance() const {
    return 8.0;
}

int BlockSpring::getCurrentFrame(const QPoint& blockPos) {
    auto& framesToEnd = frameCount[{ blockPos.x(), blockPos.y() }];
    if (framesToEnd <= 0) {
        return 0;
    } else {
        return FRAME_COUNT - (framesToEnd--);
    }
}

void BlockSpring::onStand(QPoint blockPos, Entity& entity) {
    frameCount[{ blockPos.x(), blockPos.y() }] = FRAME_COUNT;
    entity.setVelocity(entity.getVelocity() + QVector2D(0, BOUNCE_VELOCITY));
}

BoundingBox BlockSpring::getBoundingBox() const {
    return BoundingBox { { 0.1875f, 0.375f }, { 0.625f, 0.625f } };
}

QString BlockSpring::getDisplayName() const {
	return "弹簧";
}

void BlockSpring::collide(QPoint blockPos, Entity& entity, Direction dir) {
    if (dir == Direction::UP) {
        qDebug() << "Collide up";
        onStand(blockPos, entity);
    }
}

bool BlockSpring::canPlaceAt(QPoint blockPos) const {
	auto block = registry::BlockRegistry::instance().getBlockByName(World::instance().getBlock(blockPos + QPoint(0, 1)));
	return block != nullptr && block->isSolid();
}

bool BlockSpring::isSolid() const {
	return false;
}

}
