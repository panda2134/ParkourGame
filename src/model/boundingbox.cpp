#include "boundingbox.h"

namespace parkour {

bool BoundingBoxWorld::in(const BoundingBoxWorld& other) const {
    QVector2D rectOffset = position + offset,
              rectEnd = position + offset + dimensions,
              otherRectOffset = other.position + other.offset;
    return geometry::pointFallsInRectangle(rectOffset, otherRectOffset, other.dimensions)
        || geometry::pointFallsInRectangle(QVector2D(rectOffset.x(), rectEnd.y()), otherRectOffset, other.dimensions)
        || geometry::pointFallsInRectangle(QVector2D(rectEnd.x(), rectOffset.y()), otherRectOffset, other.dimensions)
        || geometry::pointFallsInRectangle(rectEnd, otherRectOffset, other.dimensions);
}

float BoundingBoxWorld::getMinX() const {
    return (position + offset).x();
}

float BoundingBoxWorld::getMaxX() const {
    return (position + offset + dimensions).x();
}

float BoundingBoxWorld::getMinY() const {
    return (position + offset).y();
}

float BoundingBoxWorld::getMaxY() const {
    return (position + offset + dimensions).y();
}

bool BoundingBoxWorld::intersect(const BoundingBoxWorld& b1, const BoundingBoxWorld& b2) {
    return b1.in(b2) || b2.in(b1);
}

}
