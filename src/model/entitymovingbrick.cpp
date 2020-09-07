#include "entitymovingbrick.h"
#include "../utils/consts.h"
#include "./boundingbox.h"
#include "world.h"

namespace parkour {
	EntityMovingBrick::EntityMovingBrick() {
		start = end = {.0f, .0f};
	}
	QString EntityMovingBrick::getName() const {
		return "moving_brick";
	}

	QString EntityMovingBrick::getResourceLocation() {
		return ":/assets/entities/moving_brick.png";
	}

	QVector2D EntityMovingBrick::getTextureDimensions() {
		return QVector2D(1.0, 1.0);
	}

	void EntityMovingBrick::setEnds(const QVector2D &start, const QVector2D &end) {
		this->start = start;
		this->end = end;
	}

	BoundingBox EntityMovingBrick::getBoundingBox() const {
		return BoundingBox{ { 0, 0 }, { 1, 1 } };
	}

	void EntityMovingBrick::update() {
		if (ticksLeft >= 0) {
			ticksLeft--;
		} else {
			switch (state) {
			case MovingState::FORWARD:
				state = MovingState::BACKWARD;
				break;
			case MovingState::BACKWARD:
			case MovingState::STOP:
				state = MovingState::FORWARD;
				break;
			}
			ticksLeft = (end - start).length() / movingVelocity * TICKS_PER_SEC;
		}
		if (state == MovingState::FORWARD) {
			setVelocity((end - start).normalized() * movingVelocity);
		} else if (state == MovingState::BACKWARD) {
			setVelocity((start - end).normalized() * movingVelocity);
		}
	}

	bool EntityMovingBrick::isAffectedByGravity() const {
		return false;
	}
	QString EntityMovingBrick::getDisplayName() const {
		return "可移动砖块";
	}
	double EntityMovingBrick::getMass() const {
		return 1e8;
	}
	void EntityMovingBrick::damage(double val) {
		/* do nothing */
	}
	bool EntityMovingBrick::showDeathAnimationAndInfo() const {
		return false;
	}
	void EntityMovingBrick::serializeCustomProps(QDataStream & out) const {
		out << ticksLeft << state << start << end;
	}
	void EntityMovingBrick::deserializeCustomProps(QDataStream & in) {
		in >> ticksLeft >> state >> start >> end;
	}
	int EntityMovingBrick::getSerializationVersion() const {
		return 2;
	}
}
