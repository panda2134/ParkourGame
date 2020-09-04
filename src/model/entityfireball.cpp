#include "entityfireball.h"
#include "world.h"
#include "../controller/worldcontroller.h"

namespace parkour {
EntityFireball::EntityFireball(double power)
    : explosionPower(power)
    , livingTicks(0) {
}

QString parkour::EntityFireball::getName() const {
    return "fireball";
}

bool parkour::EntityFireball::isAffectedByGravity() const {
    return false;
}

bool EntityFireball::isAffectedByExplosionWave() const {
    return false;
}

QString parkour::EntityFireball::getResourceLocation() {
    return ":/assets/entities/fireball.png";
}

QVector2D parkour::EntityFireball::getTextureDimensions() {
    return { 0.5, 0.5 };
}

BoundingBox parkour::EntityFireball::getBoundingBox() const {
    return BoundingBox { { 0.125f, 0.125f }, { 0.25f, 0.25f } };
}

void EntityFireball::collide(ICollidable&, Direction) {
    this->setHp(-1); // 杀死此实体且不进入 damage 回调
    WorldController::instance().explode(getPosition().toPoint(), explosionPower);
}

void EntityFireball::placeFireball(QVector2D position, QVector2D velocity, double power) {
	auto fireball = QSharedPointer<EntityFireball>::create(power);
	fireball->placeBoundingBoxAt(position);
	fireball->setVelocity(velocity);
	World::instance().addEntity(fireball);
}

void EntityFireball::serializeCustomProps(QDataStream & out) const {
	out << explosionPower << livingTicks;
}

void EntityFireball::deserializeCustomProps(QDataStream & in) {
	in >> explosionPower >> livingTicks;
}

int EntityFireball::getSerializationVersion() const {
	return 1;
}

void EntityFireball::damage(double value) {
    const double TURN_AROUND_THRESHOLD = 20;
    this->setVelocity((1 - value / TURN_AROUND_THRESHOLD) * this->getVelocity());
    Entity::damage(value); // 调用父类处理
}

bool EntityFireball::showDeathAnimationAndInfo() const {
    return false;
}

void EntityFireball::update() {
    if (++livingTicks > MAX_AGE) {
        this->setHp(-1);
    }
}

QString EntityFireball::getDisplayName() const {
	return "火球";
}


}
