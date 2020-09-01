#include "entitymovingbrick.h"
#include "../utils/consts.h"
#include "./boundingbox.h"
#include "world.h"

namespace parkour {
	double EntityMovingBrick::getWalkSpeed() const {
		return 1.0;
	}
	QString EntityMovingBrick::getName() const {
    return "moving_brick";
}

QString EntityMovingBrick::getResourceLocation() {
    return ":/assets/blocks/stone_brick.png";
}

QVector2D EntityMovingBrick::getTextureDimensions() {
    return QVector2D(1.0, 1.0);
}

BoundingBox EntityMovingBrick::getBoundingBox() const {
    return BoundingBox { { 0.1, 0.15 }, { 0.8, 0.8 } };
}

void EntityMovingBrick::update() {
    if (++ticks % 120 < 60) {
		goLeft();
	} else {
		goRight();
	}
	this->setVelocity({ this->getVelocity().x(), 0 });
}
bool EntityMovingBrick::isAffectedByGravity() const {
	return false;
}
double EntityMovingBrick::getMass() const {
	return 1e10;
}
void EntityMovingBrick::serializeCustomProps(QDataStream & out) const {
	out << ticks;
}
void EntityMovingBrick::deserializeCustomProps(QDataStream & in) {
	in >> ticks;
}
int EntityMovingBrick::getSerializationVersion() const {
	return 1;
}
}
