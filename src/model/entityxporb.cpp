#include "entityxporb.h"
#include "entityplayer.h"
#include "world.h"
#include <QSharedPointer>
#include <QRandomGenerator>

namespace parkour {
	EntityXpOrb::EntityXpOrb()
		: xp(0), tick(0) {}
	void EntityXpOrb::serializeCustomProps(QDataStream & out) const {
		out << xp;
	}
	void EntityXpOrb::deserializeCustomProps(QDataStream & in) {
		in >> xp;
	}
	int EntityXpOrb::getSerializationVersion() const {
		return 1;
	}
	QString EntityXpOrb::getName() const {
		return "xp_orb";
	}
	QString EntityXpOrb::getResourceLocation() {
		return ":/assets/entities/xp_orb.png";
	}
	QVector2D EntityXpOrb::getTextureDimensions() {
		return QVector2D(0.5, 0.5);
	}
	BoundingBox EntityXpOrb::getBoundingBox() const {
		return BoundingBox{ {0.0625f, 0.0625f}, {0.125f, 0.125f} };
	}
	QRect EntityXpOrb::getTextureRenderRect() const {
		const int size = 64;
		int level = std::clamp(this->xp / 3, 0, 7);
		int frame = tick % 32;
		return QRect(QPoint(level * size, frame * size), QSize(size, size));
	}
	void EntityXpOrb::update() {
		++tick;
		const float DRAG_FACTOR = 0.1;
		if (isOnFloor() && tick > TICKS_PER_SEC / 4.0f) {
			setVelocity(getVelocity() * DRAG_FACTOR);
		}
	}
	void EntityXpOrb::collide(ICollidable & other, Direction dir) {
		if (other.getType() == "entity" && other.getName() == "player") {
			auto &player = static_cast<EntityPlayer&>(other);
			player.increaseExp(xp);
			this->setHp(-1);
		}
	}
	bool EntityXpOrb::showDeathAnimation() const {
		return false;
	}
	bool EntityXpOrb::isAffectedByExplosionWave() const {
		return false;
	}
	void EntityXpOrb::setXp(int val) {
		xp = val;
	}
	void EntityXpOrb::damage(double val) {
		// do not damage
	}
	double EntityXpOrb::getMass() const {
		return 0.1;
	}
	void EntityXpOrb::dropXpOrbs(QVector2D position, int xp) {
		auto orb = QSharedPointer<EntityXpOrb>::create();
		auto gen = QRandomGenerator::global();
		orb->setXp(xp);
		orb->placeBoundingBoxAt(position + QVector2D(gen->generateDouble() * 2, 0));
		orb->setVelocity(QVector2D(gen->generateDouble(), gen->generateDouble()) * 3);
		World::instance().addEntity(orb);
	}
}