#ifndef GEOMETRYHELPER_H
#define GEOMETRYHELPER_H

#include <QVector2D>
#include <QtMath>

namespace parkour {
namespace geometry {
    int compareDoubles(double lhs, double rhs);
    double normalizeDenominater(double d);
    bool pointFallsInRectangle(const QVector2D& point, const QVector2D& rectOffset, const QVector2D& rectDimensions);
}
}

#endif // GEOMETRYHELPER_H
