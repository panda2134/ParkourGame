#include "entity.h"

namespace parkour {

Entity::Entity(QObject* parent)
    : QObject(parent) {
}

QVector2D Entity::getPosition() const { return position; }

void Entity::setPosition(const QVector2D& value) {
    position = value;
}

double Entity::getHp() const {
    return hp;
}

void Entity::setHp(double value) {
    hp = value;
    if (hp < 0) {
        this->dying = true;
    }
}

void Entity::damage(double value) {
    setHp(getHp() - value);
}

bool Entity::isDying() const {
    return dying;
}

bool Entity::isOnFloor() const {
    return onFloor;
}

void Entity::setOnFloor(bool value) {
    onFloor = value;
}

void Entity::updatePosition() {
    // 计算一个tick内的运动学变化
    this->setVelocity(this->getVelocity() + TICK_LENGTH * this->getAcceleration());
    this->setPosition(this->getPosition() + TICK_LENGTH * this->getVelocity());
}

void Entity::placeBoundingBoxAt(const QVector2D& bottomLeft) {
    const auto& bboxWorld = this->getBoundingBoxWorld();
    this->setPosition(bottomLeft - QVector2D(0, bboxWorld.dimensions.y()) - bboxWorld.offset);
    qDebug("MaxY=%.6f", this->getBoundingBoxWorld().getMaxY());
}

QVector2D Entity::getBoundingBoxBottomLeft() {
    const auto& bboxWorld = this->getBoundingBoxWorld();
    return this->getPosition() + bboxWorld.offset + QVector2D(0, bboxWorld.dimensions.y());
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
