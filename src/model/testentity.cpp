#include "testentity.h"
#include "../controller/worldcontroller.h"
#include "../utils/consts.h"
#include "./boundingbox.h"
#include "world.h"

namespace parkour {

QString TestEntity::getName() const {
    return "test";
}

QString TestEntity::getResourceLocation() {
    return ":/assets/entities/test.png";
}

QVector2D TestEntity::getTextureDimensions() {
    return QVector2D(1.0, 2.0);
}

BoundingBox TestEntity::getBoundingBox() const {
    return BoundingBox { { 0.05, 0.05 }, { 0.9, 1.9 } };
}

void TestEntity::update() {
    goRight();
    if (World::instance().getTicksFromBirth() / TICKS_PER_SEC > 3 && World::instance().getTicksFromBirth() % TICKS_PER_SEC == 0) {
        WorldController::instance().explode((getPosition()).toPoint(), 7);
    }
}
}
