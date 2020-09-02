#include "entityslime.h"
#include "entityplayer.h"
#include "entityxporb.h"
#include "world.h"
#include <QRandomGenerator>

namespace parkour {
	EntitySlime::EntitySlime(): waitTicksLeft(-1) {}
	QString EntitySlime::getName() const {
		return "slime";
	}
	QString EntitySlime::getResourceLocation() {
		return ":/assets/entities/slime.png";
	}
	QVector2D EntitySlime::getTextureDimensions() {
		return QVector2D(1.0f, 1.0f);
	}
	BoundingBox EntitySlime::getBoundingBox() const {
		return BoundingBox{ {0.125f, 0.125f}, {0.75f, 0.75f} };
	}
	double EntitySlime::getWalkSpeed() const {
		return 1.5;
	}
	double EntitySlime::getMass() const {
		return 6000;
	}

	void EntitySlime::collide(ICollidable & other, Direction dir) {
		if (dir == Direction::DOWN) {
			this->waitTicksLeft = WAIT_TIMEOUT_MIN + (WAIT_TIMEOUT_MAX - WAIT_TIMEOUT_MIN) * QRandomGenerator::global()->generateDouble();
			qDebug() << this->getVelocity().y();
			this->setVelocity({ .0f, -0.1f * getVelocity().y() });
			if (other.getType() == "entity") {
				this->setVelocity({ .0f, -JUMP_SPEED });
			}
		}
		if (other.getName() == "player") {
			auto &player = static_cast<EntityPlayer&>(other);
			player.damage(DAMAGE_PER_HIT + (player.getVelocity() - getVelocity()).length() * COLLISION_MULTIPLIER);
		}
	}
	void EntitySlime::update() {
		if (waitTicksLeft >= 0) {
			waitTicksLeft--;
		} else {
			if (!isOnFloor()) {
				return;
			}
			auto &world = World::instance();
			for (auto &entity : world.getEntities()) {
				if (entity->getName() == "player") {
					if (entity->getPosition().x() < getPosition().x()) {
						jump(); goLeft();
					} else {
						jump(); goRight();
					}
					break;
				}
			}
		}
	}
	void EntitySlime::damage(double value) {
		EntityPlayerLike::damage(value);
		if (this->getHp() < 0) {
			auto gen = QRandomGenerator::global();
			int count = gen->generate() % 3 + 1;
			for (int i = 0; i < count; i++) {
				EntityXpOrb::dropXpOrbs(getPosition(), gen->generateDouble() * 24);
			}
		}
	}
	void EntitySlime::serializeCustomProps(QDataStream & out) const {
		out << waitTicksLeft;
	}
	void EntitySlime::deserializeCustomProps(QDataStream & in) {
		in >> waitTicksLeft;
	}
	int EntitySlime::getSerializationVersion() const {
		return 1;
	}
}