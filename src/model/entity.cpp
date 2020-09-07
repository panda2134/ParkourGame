#include "entity.h"
#include "../utils/consts.h"
#include "../utils/geometryhelper.h"

namespace parkour {

Entity::Entity(QObject* parent) : QObject(parent) {
	this->setHp(this->getMaxHp());
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

void Entity::regenerate() {
	this->setHp(this->getMaxHp());
}

void Entity::damage(double value) {
    setHp(getHp() - value);
}

double Entity::getMaxHp() const {
	return 20.0;
}

QRect Entity::getTextureRenderRect() const {
	return QRect();
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

BoundingBoxWorld Entity::getBoundingBoxWorld() const {
    QVector2D blockPosVect(this->getPosition());
    BoundingBox bbox = this->getBoundingBox();
    BoundingBoxWorld ret(blockPosVect, bbox);
    return ret;
}

QString Entity::getType() const {
	return "entity";
}

void Entity::serializeEntityBasics(QDataStream & out) const {
	out << position << velocity << acceleration << hp << dying << onFloor;
}

void Entity::deserializeEntityBasics(QDataStream & in) {
	in >> position >> velocity >> acceleration >> hp >> dying >> onFloor;
}

void Entity::updatePosition() {
    // 计算一个tick内的运动学变化
    this->setVelocity(this->getVelocity() + TICK_LENGTH * this->getAcceleration());
    this->setPosition(this->getPosition() + TICK_LENGTH * this->getVelocity());
}

bool Entity::showDeathAnimationAndInfo() const {
	return true;
}

bool Entity::isAffectedByGravity() const {
    return true;
}

bool Entity::isAffectedByExplosionWave() const {
    return true;
}

void Entity::placeBoundingBoxAt(const QVector2D& bottomLeft) {
    const auto& bboxWorld = this->getBoundingBoxWorld();
    this->setPosition(bottomLeft - QVector2D(0, bboxWorld.dimensions.y()) - bboxWorld.offset);
}

QVector2D Entity::getBoundingBoxBottomLeft() {
    const auto& bboxWorld = this->getBoundingBoxWorld();
    return this->getPosition() + bboxWorld.offset + QVector2D(0, bboxWorld.dimensions.y());
}

QVector2D Entity::getVelocity() const { return velocity; }

void Entity::setVelocity(const QVector2D& value) {
    velocity = QVector2D(geometry::compareDoubles(value[0], 0) * qMin(1.0 * qAbs(value[0]), MAX_VELOCITY),
        geometry::compareDoubles(value[1], 0) * qMin(1.0 * qAbs(value[1]), MAX_VELOCITY));
	if (qAbs(velocity[0]) < ENTITY_MIN_VELOCITY) { velocity[0] = 0; }
	if (qAbs(velocity[1]) < ENTITY_MIN_VELOCITY) { velocity[1] = 0; }
}

QVector2D Entity::getAcceleration() const {
    return acceleration;
}

void Entity::setAcceleration(const QVector2D& value) {
    acceleration = QVector2D(geometry::compareDoubles(value[0], 0) * qMin(1.0 * qAbs(value[0]), MAX_ACCELERATION),
        geometry::compareDoubles(value[1], 0) * qMin(1.0 * qAbs(value[1]), MAX_ACCELERATION));
}

QDataStream & operator<<(QDataStream & out, const Entity & e) {
	out << e.getSerializationVersion();
	e.serializeEntityBasics(out);
	e.serializeCustomProps(out);
	return out;
}

QDataStream & operator>>(QDataStream & in, Entity & e) {
	in.startTransaction();
	int version; in >> version;
	if (version != e.getSerializationVersion()) {
		in.abortTransaction();
	} else {
		e.deserializeEntityBasics(in);
		e.deserializeCustomProps(in);
		in.commitTransaction();
	}
	return in;
}

}
