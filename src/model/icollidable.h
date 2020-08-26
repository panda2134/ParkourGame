#ifndef ICOLLIDABLE_H
#define ICOLLIDABLE_H

#include "boundingbox.h"

namespace parkour {
    class ICollidable {
    public:
        /**
         * @brief getBoundingBoxWorld
         * @return 世界坐标下的绑定盒
         */
        virtual BoundingBoxWorld getBoundingBoxWorld() const = 0;
        virtual void collide(const ICollidable&, Direction) = 0;
        virtual QVector2D getVelocity() const = 0;
        virtual Direction checkCollideWith(const ICollidable&) final;
    };
}

#endif // ICOLLIDABLE_H
