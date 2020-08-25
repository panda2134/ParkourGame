#include "./geometryhelper.h"

namespace parkour {
namespace geometry {

    int compareDoubles(double lhs, double rhs) {
        using parkour::EPSILON;
        return qAbs(lhs - rhs) < EPSILON ? 0 : (lhs - rhs > EPSILON ? 1 : -1);
    }

    /**
     * @brief pointFallsInRectangle 判断点是否严格在矩形内
     * @param point 点
     * @param rectOffset 矩形左上角坐标
     * @param rectDimensions 矩形x, y方向的长宽
     * @return 判断结果
     */
    bool pointFallsInRectangle(const QVector2D& point, const QVector2D& rectOffset, const QVector2D& rectDimensions) {
        const QVector2D rectEnd = rectOffset + rectDimensions;
        return (compareDoubles(point.x(), rectOffset.x()) > 0 && compareDoubles(point.x(), rectEnd.x()) < 0)
            && (compareDoubles(point.y(), rectOffset.y()) > 0 && compareDoubles(point.y(), rectEnd.y()) < 0);
    }

    double normalizeDenominater(double d) {
        return qAbs(d) >= EPSILON ? d : (EPSILON * (d > 0 ? 1 : -1));
    }
}
}
