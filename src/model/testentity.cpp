#include "testentity.h"

namespace parkour {

QString TestEntity::getName() const {
    return "test";
}

QString TestEntity::getResourceLocation() {
    return ":/assets/entities/test.png";
}

BoundingBox TestEntity::getBoundingBox() const {
    BoundingBox ret;
    ret.offset = offset;
    ret.dimensions = dimensions;
    return ret;
}

BoundingBoxWorld TestEntity::getBoundingBoxWorld() const {
    return BoundingBoxWorld(this->getPosition(), this->getBoundingBox());
}

void TestEntity::collide(const ICollidable& oth, Direction dir) {
    qDebug() << dir;
}
}
