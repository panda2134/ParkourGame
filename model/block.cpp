#include "block.h"

namespace parkour {

int Block::getCurrentFrame(const QPoint& blockPos) {
    return -1;
}

void Block::onExplosion(QPoint, double) {
}

void Block::onStand(QPoint blockPos, Entity & entity) {}

bool Block::canPlaceAt(QPoint) const {
	return true;
}

bool Block::isSolid() const {
	return true;
}

BoundingBox Block::getBoundingBox() const {
    BoundingBox ret;
    ret.offset = QVector2D(.0, .0);
    ret.dimensions = QVector2D(1., 1.);
    return ret;
}

}
