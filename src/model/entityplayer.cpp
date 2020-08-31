#include "entityplayer.h"

namespace parkour {

EntityPlayer::EntityPlayer()
    : EntityPlayerLike() {
    this->setHp(20.0);
	this->maxHp = 20.0;
	this->shootFireballs = false;
}

BoundingBox EntityPlayer::getBoundingBox() const {
    if (isSneak()) {
        return BoundingBox { QVector2D(0.25, 1.00), QVector2D(0.5, 0.98) };
    } else {
        return BoundingBox { QVector2D(0.25, 0.15), QVector2D(0.5, 1.85) };
    }
}

QString EntityPlayer::getName() const {
    return "player";
}

QString EntityPlayer::getResourceLocation() {
    if (!isSneak()) {
        return ":/assets/entities/player/0.png";
    } else {
        return ":/assets/entities/player/1.png";
    }
}

QVector2D EntityPlayer::getTextureDimensions() {
    return QVector2D(1, 2);
}

void EntityPlayer::update() {
	if (this->isOnFloor()) {
		const float vx = this->getVelocity().x();
		if (qAbs(vx) < GROUND_STOP_THRESHOLD) {
			this->setVelocity({ 0.0f, this->getVelocity().y() });
			this->setAcceleration({ 0.0f, this->getAcceleration().y() });
		} else {
			this->setAcceleration({ static_cast<float>(-1 * DRAG_FACTOR * vx * vx * vx / qAbs(vx)), this->getAcceleration().y() });
		}
	}
}

void EntityPlayer::setMaxHp(double value) {
	maxHp = value;
}

double EntityPlayer::getMaxHp() const {
	return maxHp;
}

bool EntityPlayer::isAbleToShootFireballs() {
	return shootFireballs;
}

void EntityPlayer::setShootFireballs(bool value) {
	this->shootFireballs = value; 
}

}
