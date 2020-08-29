#ifndef BOUNDINGBOX_H
#define BOUNDINGBOX_H

#include <QVector2D>

namespace parkour {
    struct BoundingBox {
        /**
         * 一个角的坐标
         */
        QVector2D offset;
        /**
         * 从坐标开始的增量dx, dy；边界盒是从 (x, y) 到 (x+dx, y+dy) 的矩形
         */
        QVector2D dimensions;
    };
    struct BoundingBoxWorld : public BoundingBox {
        /**
         * 世界坐标
         */
        QVector2D position;

        BoundingBoxWorld(QVector2D position_, BoundingBox box_)
            : position(position_) {
            this->offset = box_.offset;
            this->dimensions = box_.dimensions;
        }

        /**
         * @brief in 两个碰撞箱严格相交
         * @param other 另一个世界碰撞箱
         * @return 是否严格相交
         */
        bool in(const BoundingBoxWorld& other) const;
        bool standUpon(const BoundingBoxWorld& other) const;
        float getMinX() const;
        float getMaxX() const;
        float getMinY() const;
        float getMaxY() const;
        static bool intersect(const BoundingBoxWorld& b1, const BoundingBoxWorld& b2);
    };
}

#endif // BOUNDINGBOX_H
