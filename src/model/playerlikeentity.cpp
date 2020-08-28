#include "playerlikeentity.h"

namespace parkour {

void PlayerLikeEntity::collide(const ICollidable& other, Direction dir) {
    Q_UNUSED(other)
    if (dir == Direction::DOWN) {
        this->damage(qMax(.0, (this->getVelocity().y() - FALL_DAMAGE_SPEED) * FALL_DAMAGE_MULTIPLIER));
    }
}

void PlayerLikeEntity::jump() {
    if (this->isOnFloor()) {
        this->setVelocity(QVector2D(this->getVelocity().x(), -JUMP_SPEED));
    }
}
void PlayerLikeEntity::goLeft() {
    auto vx = qMin(1.0 * getVelocity().x(), -1 * WALK_SPEED);
    setVelocity(QVector2D(vx, getVelocity().y()));
}

void PlayerLikeEntity::goRight() {
    auto vx = qMax(1.0 * getVelocity().x(), WALK_SPEED);
    setVelocity(QVector2D(vx, getVelocity().y()));
}
}
