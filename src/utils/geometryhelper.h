#ifndef GEOMETRYHELPER_H
#define GEOMETRYHELPER_H

#include "consts.h"
#include <QVector2D>
#include <QtMath>

namespace parkour {
namespace geometry {
    int compareFloats(float lhs, float rhs);
    bool pointFallsInRectangle(const QVector2D& point, const QVector2D& rectOffset, const QVector2D& rectDimensions);
}
}

#endif // GEOMETRYHELPER_H
