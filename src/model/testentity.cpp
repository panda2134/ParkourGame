#include "testentity.h"

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
    BoundingBox ret;
    ret.offset = QVector2D(0.05, 0.05);
    ret.dimensions = QVector2D(0.9, 1.9);
    return ret;
}

BoundingBoxWorld TestEntity::getBoundingBoxWorld() const {
    return BoundingBoxWorld(this->getPosition(), this->getBoundingBox());
}

void TestEntity::update() {
    if (World::instance().getTicksFromBirth() / TICKS_PER_SEC == 3 && World::instance().getTicksFromBirth() % TICKS_PER_SEC == 0) {
        LocalWorldController::instance().explode((getPosition()).toPoint(), 7);
    }
}

void TestEntity::collide(const ICollidable& other, Direction dir) {
    PlayerLikeEntity::collide(other, dir);
    qDebug() << this->getVelocity();
}
}
