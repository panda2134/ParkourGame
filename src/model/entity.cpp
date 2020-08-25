#include "entity.h"

namespace parkour {

Entity::Entity(QObject* parent)
    : QObject(parent) {
    position.setX(0);
    position.setY(0);
    velocity.setX(0);
    velocity.setY(0);
    acceleration.setX(0);
    acceleration.setY(0);
}

QVector2D Entity::getPosition() const { return position; }

void Entity::setPosition(const QVector2D& value) {
    position = value;
}

void Entity::updatePosition() {
    // 计算一个tick内的运动学变化
    this->setVelocity(this->getVelocity() + TICK_LENGTH * this->getAcceleration());
    this->setPosition(this->getPosition() + TICK_LENGTH * this->getVelocity());
}

QVector2D Entity::getVelocity() const { return velocity; }

void Entity::setVelocity(const QVector2D& value) {
    velocity = QVector2D(geometry::compareDoubles(value[0], 0) * qMin(1.0 * qAbs(value[0]), MAX_VELOCITY),
        geometry::compareDoubles(value[1], 0) * qMin(1.0 * qAbs(value[1]), MAX_VELOCITY));
}

QVector2D Entity::getAcceleration() const {
    return acceleration;
}

void Entity::setAcceleration(const QVector2D& value) {
    acceleration = QVector2D(geometry::compareDoubles(value[0], 0) * qMin(1.0 * qAbs(value[0]), MAX_ACCELERATION),
        geometry::compareDoubles(value[1], 0) * qMin(1.0 * qAbs(value[1]), MAX_ACCELERATION));
}

}
