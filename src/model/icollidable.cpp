#include "./icollidable.h"
#include "../utils/consts.h"
#include "../utils/geometryhelper.h"

namespace parkour {
Direction ICollidable::checkCollideWith(const ICollidable& other) {
    if (!BoundingBoxWorld::intersect(this->getBoundingBoxWorld(), other.getBoundingBoxWorld())) {
        return Direction::UNKNOWN;
    }
    QVector2D relativeVelocity = this->getVelocity() - other.getVelocity();
    QVector2D deltaPosition = TICK_LENGTH * relativeVelocity;
    Direction dir = Direction::UNKNOWN;
    // Judge Down & Up
    const float deltaDown = this->getBoundingBoxWorld().getMaxY() - other.getBoundingBoxWorld().getMinY();
    const float deltaUp = other.getBoundingBoxWorld().getMaxY() - this->getBoundingBoxWorld().getMinY();
    qDebug() << deltaDown << deltaUp;
    if (deltaDown > 0 || deltaUp > 0) {
        const float ratio = qAbs((deltaDown < deltaUp ? deltaDown : deltaUp) / deltaPosition.y());
        const float left = this->getBoundingBoxWorld().getMinX() - deltaPosition.x() * ratio,
                    right = this->getBoundingBoxWorld().getMaxX() - deltaPosition.x() * ratio;
        if (geometry::compareDoubles(other.getBoundingBoxWorld().getMaxX(), left) < 0
            || geometry::compareDoubles(other.getBoundingBoxWorld().getMinX(), right) > 0) {
            /* 不是从此方向碰撞，do nothing */
        } else {
            dir = deltaDown < deltaUp ? Direction::DOWN : Direction::UP;
        }
    }
    // Judge Left & Right
    const float deltaRight = this->getBoundingBoxWorld().getMaxX() - other.getBoundingBoxWorld().getMinX();
    const float deltaLeft = other.getBoundingBoxWorld().getMaxX() - this->getBoundingBoxWorld().getMinX();
    qDebug() << deltaRight << deltaLeft;
    if ((deltaRight > 0 || deltaLeft > 0) && geometry::compareDoubles(qMin(deltaLeft, deltaRight), qMin(deltaUp, deltaDown)) < 0) {
        const float ratio = qAbs((deltaRight < deltaLeft ? deltaRight : deltaLeft) / deltaPosition.x());
        const float up = this->getBoundingBoxWorld().getMinY() - deltaPosition.y() * ratio,
                    down = this->getBoundingBoxWorld().getMaxY() - deltaPosition.y() * ratio;
		qDebug() << ">" << up << down << other.getBoundingBoxWorld().getMaxY() << other.getBoundingBoxWorld().getMinY();
        if (geometry::compareDoubles(other.getBoundingBoxWorld().getMaxY(), up) < 0
            || geometry::compareDoubles(other.getBoundingBoxWorld().getMinY(), down) > 0) {
            /* 不是从此方向碰撞，do nothing */
        } else {
            dir = deltaRight < deltaLeft ? Direction::RIGHT : Direction::LEFT;
        }
    }
	qDebug() << "DIR =" << dir;
    return dir;
}
}
