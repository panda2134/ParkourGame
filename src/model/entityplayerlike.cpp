#include "entityplayerlike.h"
#include "../utils/consts.h"
#include "blockdelegate.h"
#include "registry.h"
#include "world.h"

namespace parkour {
	double EntityPlayerLike::getWalkSpeed() const {
		return WALK_SPEED;
	}
	EntityPlayerLike::EntityPlayerLike() {
    sneak = false;
}

void EntityPlayerLike::collide(ICollidable& other, Direction dir) {
    Q_UNUSED(other)
    if (dir == Direction::DOWN) {
        this->damage(qMax(.0, (this->getVelocity().y() - FALL_DAMAGE_SPEED) * FALL_DAMAGE_MULTIPLIER));
    }
}

void EntityPlayerLike::jump() {
    if (this->isOnFloor()) {
        this->setVelocity(QVector2D(this->getVelocity().x(), -JUMP_SPEED));
    }
}

void EntityPlayerLike::goLeft() {
    auto vx = qMin(1.0 * getVelocity().x(), -1 * getWalkSpeed());
    setVelocity(QVector2D(vx, getVelocity().y()));
}

void EntityPlayerLike::goRight() {
    auto vx = qMax(1.0 * getVelocity().x(), getWalkSpeed());
    setVelocity(QVector2D(vx, getVelocity().y()));
}

bool EntityPlayerLike::isSneak() const {
    return sneak;
}

bool EntityPlayerLike::setSneak(bool value) {
    auto& world = World::instance();
    sneak = value;
    if (world.isReady()) { // 检查：下一个状态的碰撞盒是否可能嵌入其他物体？
        const auto& newBoundingBoxWorld = getBoundingBoxWorld();
        bool fail = false;

        for (int i = getPosition().x() - ENTITY_COLLISION_RANGE; i <= getPosition().x() + ENTITY_COLLISION_RANGE; i++) {
            for (int j = getPosition().y() - ENTITY_COLLISION_RANGE; j <= getPosition().y() + ENTITY_COLLISION_RANGE; j++) {
                const auto blockName = world.getBlock({ i, j });
                if (blockName == "air") {
                    continue;
                }
                if (newBoundingBoxWorld.in(BlockDelegate(blockName, { i, j }).getBoundingBoxWorld())) {
                    fail = true;
                    goto fail;
                }
            }
        }

        for (auto& entity : world.getEntities()) {
            if (entity == this) {
                continue;
            }
            if (newBoundingBoxWorld.in(entity->getBoundingBoxWorld())) {
                fail = true;
                break;
            }
        }

    fail:
        if (fail) { // 碰撞，还原原来的状态
            sneak = !value;
        }
    }
    return sneak;
}
}
