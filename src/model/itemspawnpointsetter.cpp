#include "itemspawnpointsetter.h"
#include "world.h"

namespace parkour {
	ItemSpawnPointSetter::ItemSpawnPointSetter() {
		icon.load(":/assets/items/spawnpoint.png");
	}
	QString ItemSpawnPointSetter::getDisplayName() {
		return "出生点设置器";
	}
	const QImage & ItemSpawnPointSetter::getIcon() {
		return icon;
	}
	void ItemSpawnPointSetter::onUse(QVector2D usePosition) {
		World::instance().setSpawnPoint(usePosition);
	}
}