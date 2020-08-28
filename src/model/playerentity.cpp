#include "playerentity.h"

namespace parkour {

PlayerEntity::PlayerEntity() {
    this->setHp(20.0);
    this->setSneak(false);
}

bool PlayerEntity::isSneak() const {
    return sneak;
}

void PlayerEntity::setSneak(bool value) {
    sneak = value;
}

BoundingBox PlayerEntity::getBoundingBox() const {
    if (sneak) {
        return BoundingBox { QVector2D(0.05, 1.05), QVector2D(0.9, 0.9) };
    } else {
        return BoundingBox { QVector2D(0.05, 0.05), QVector2D(0.9, 1.9) };
    }
}

QString PlayerEntity::getName() const {
    return "player";
}

QString PlayerEntity::getResourceLocation() {
    if (!sneak) {
        return ":/assets/entities/player.png";
    } else {
        return ":/assets/entities/player-sneak.png";
    }
}

QVector2D PlayerEntity::getTextureDimensions() {
    return QVector2D(1, 2);
}

}
