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
	if (ticksLeft >= 0) {
		ticksLeft--;
	} else {
		switch (state) {
		case MovingState::STOP:
		case MovingState::LEFT:
			state = MovingState::RIGHT;
			break;
		case MovingState::RIGHT:
			state = MovingState::LEFT;
			break;
		}
		ticksLeft = TICKS_PER_SEC * 1.2;
	}
	if (state == MovingState::LEFT) {
		goLeft();
	} else if (state == MovingState::RIGHT) {
		goRight();
	}
	this->setVelocity({ this->getVelocity().x(), 0 });
}



bool EntityMovingBrick::isAffectedByGravity() const {
	return false;
}
QString EntityMovingBrick::getDisplayName() const {
	return "可移动砖块";
}
double EntityMovingBrick::getMass() const {
	return 1e10;
}
void EntityMovingBrick::collide(ICollidable & other, Direction dir) {
	if (dir == Direction::LEFT && state != MovingState::RIGHT) {
		state = MovingState::RIGHT;
	} else if (dir == Direction::RIGHT && state != MovingState::LEFT) {
		state = MovingState::LEFT;
	}
}
void EntityMovingBrick::serializeCustomProps(QDataStream & out) const {
	out << ticksLeft << state;
}
void EntityMovingBrick::deserializeCustomProps(QDataStream & in) {
	in >> ticksLeft >> state;
}
int EntityMovingBrick::getSerializationVersion() const {
	return 1;
}
}
