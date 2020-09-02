#include "blockoakplank.h"

namespace parkour {
	QString BlockOakPlank::getName() const {
		return "oak_plank";
	}
	QString BlockOakPlank::getResourceLocation() const {
		return ":/assets/blocks/oak_plank.png";
	}
	double BlockOakPlank::getExplosionResistance() const {
		return 5.0;
	}
	QString BlockOakPlank::getDisplayName() const {
		return "橡木木板";
	}
}