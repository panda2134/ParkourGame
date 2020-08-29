#include "entitytnt.h"
#include "../controller/worldcontroller.h"

namespace parkour {
EntityTNT::EntityTNT() {
    ticksLeft = TICKS_BEFORE_EXPLOSION;
}

QString EntityTNT::getName() const {
    return "tnt";
}

void EntityTNT::update() {
    ticksLeft--;
    if (ticksLeft < 0) {
        WorldController::instance().explode(getPosition().toPoint(), TNT_EXPLOSION_POWER);
    }
}

void EntityTNT::damage(double value) {
    Q_UNUSED(value)
    return; // 不接受伤害
}

QString EntityTNT::getResourceLocation() {
    return ":/assets/entities/tnt.png";
}

QVector2D EntityTNT::getTextureDimensions() {
    return { 1, 1 };
}

BoundingBox EntityTNT::getBoundingBox() const {
    return BoundingBox { QVector2D(0.1, 0.2), QVector2D(0.8, 0.8) };
}
}
