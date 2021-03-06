#include "boundingbox.h"
#include "../utils/consts.h"
#include "../utils/direction.h"
#include "../utils/geometryhelper.h"

namespace parkour {

bool BoundingBoxWorld::in(const BoundingBoxWorld& other) const {
    float xMin = qMax(getMinX(), other.getMinX()),
          yMin = qMax(getMinY(), other.getMinY()),
          xMax = qMin(getMaxX(), other.getMaxX()),
          yMax = qMin(getMaxY(), other.getMaxY());
    return geometry::compareDoubles(xMin, xMax) < 0
        && geometry::compareDoubles(yMin, yMax) < 0
        && qMax(xMax - xMin, yMax - yMin) > INTERSECT_DELTA;
}

bool BoundingBoxWorld::standUpon(const BoundingBoxWorld& other) const {
    float xMin = qMax(getMinX(), other.getMinX()),
          xMax = qMin(getMaxX(), other.getMaxX());
    return qAbs(getMaxY() - other.getMinY()) <= INTERSECT_DELTA && geometry::compareDoubles(xMax, xMin) > 0;
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

QVector2D BoundingBoxWorld::getCenter() const {
    return position + offset + 0.5 * dimensions;
}

bool BoundingBoxWorld::intersect(const BoundingBoxWorld& b1, const BoundingBoxWorld& b2) {
    return b1.in(b2) || b2.in(b1);
}

}
