#include "blockexpbottle.h"
#include "entityxporb.h"
#include "world.h"
#include "view/scene/gamesound.h"
#include <QRandomGenerator>

QString parkour::BlockExpBottle::getName() const {
	return "xp_bottle";
}

QString parkour::BlockExpBottle::getResourceLocation() const {
	return ":/assets/blocks/xp_bottle.png";
}

double parkour::BlockExpBottle::getExplosionResistance() const {
	return 1.0;
}

QString parkour::BlockExpBottle::getDisplayName() const {
	return "经验瓶";
}

void parkour::BlockExpBottle::collide(QPoint blockPos, Entity& entity, Direction) {
	if (entity.getName() != "player") {
		return;
	}
	const double MIN_XP = 1.0, MAX_XP = 2.0;
	const int MIN_COUNT = 3, MAX_COUNT = 8;
	auto gen = QRandomGenerator::system();
	int count = gen->generateDouble() * (MAX_COUNT - MIN_COUNT) + MIN_COUNT;
	for (int i = 0; i < count; i++) {
		auto xp = gen->generateDouble() * (MAX_XP - MIN_XP) + MIN_XP;
		EntityXpOrb::dropXpOrbs(QVector2D(blockPos - QPoint(0, 1)), xp);
	}
	World::instance().setBlock(blockPos, "air");
	GameSound::instance().playWorldSound(QString("Glass_dig%1").arg(gen->generate() % 3 + 1), QVector2D(blockPos));
}

void parkour::BlockExpBottle::onStand(QPoint blockPos, Entity& entity) {
	collide(blockPos, entity, Direction::UP);
}
