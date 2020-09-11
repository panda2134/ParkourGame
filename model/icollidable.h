#ifndef ICOLLIDABLE_H
#define ICOLLIDABLE_H

#include "../utils/direction.h"
#include "boundingbox.h"
#include <QDebug>

namespace parkour {
class ICollidable {
public:
    /**
         * @brief getBoundingBoxWorld
         * @return 世界坐标下的绑定盒
         */
    virtual BoundingBoxWorld getBoundingBoxWorld() const = 0;
    virtual void collide(ICollidable&, Direction) = 0;
    virtual QVector2D getVelocity() const = 0;
    virtual void setVelocity(const QVector2D&) = 0;
    virtual Direction checkCollideWith(const ICollidable&) final;
    virtual double getMass() const;
    virtual QString getName() const = 0;
    virtual QString getType() const = 0;
};
}

#endif // ICOLLIDABLE_H
