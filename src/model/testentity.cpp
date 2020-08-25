#include "testentity.h"

namespace parkour {

BoundingBox parkour::TestEntity::getBoundingBox() const {
    BoundingBox ret;
    ret.offset = QVector2D(0, 0);
    ret.dimensions = QVector2D(30, 60);
    return ret;
}

BoundingBoxWorld parkour::TestEntity::getBoundingBoxWorld() const {
    return BoundingBoxWorld(this->getPosition(), this->getBoundingBox());
}

}
