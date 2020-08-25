#include "testentity.h"

namespace parkour {

BoundingBox TestEntity::getBoundingBox() const {
    BoundingBox ret;
    ret.offset = QVector2D(0, 0);
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
