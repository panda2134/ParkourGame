#include "itemteleporter.h"
#include "../controller/worldcontroller.h"

namespace parkour {
	ItemTeleporter::ItemTeleporter() {
		icon.load(":/assets/items/tp.png");
	}
	QString ItemTeleporter::getDisplayName() {
		return "传送器";
	}
	const QImage & ItemTeleporter::getIcon() {
		return icon;
	}
	void ItemTeleporter::onUse(QVector2D usePosition) {
		auto player = WorldController::instance().getPlayerController()->getPlayer();
		if (player != nullptr) {
			player->placeBoundingBoxAt(usePosition);
			player->setVelocity({ 0, 0 });
			player->setAcceleration({ 0, 0 });
		}
	}
}
