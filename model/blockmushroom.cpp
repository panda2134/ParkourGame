#include "blockmushroom.h"
#include "entityplayer.h"
#include "world.h"
#include "registry.h"

namespace parkour {
	QString BlockMushroom::getName() const {
		return "mushroom";
	}
	QString BlockMushroom::getResourceLocation() const {
		return ":/assets/blocks/mushroom.png";
	}
	double BlockMushroom::getExplosionResistance() const {
		return 0.5;
	}
	void BlockMushroom::collide(QPoint blockPos, Entity & entity, Direction dir) {
		if (entity.getName() == "player") {
			auto &player = static_cast<EntityPlayer&>(entity);
			player.setMaxHp(player.getMaxHp() + HP_BOOST);
			player.regenerate();
			World::instance().setBlock(blockPos, "air");
		}
	}
	BoundingBox BlockMushroom::getBoundingBox() const {
		return BoundingBox{ {0.25f, 0.5625f}, {0.5f, 0.4375f} };
	}
	bool BlockMushroom::canPlaceAt(QPoint blockPos) const {
		auto block = registry::BlockRegistry::instance().getBlockByName(World::instance().getBlock(blockPos + QPoint(0, 1)));
		return block != nullptr && block->isSolid();
	}
	bool BlockMushroom::isSolid() const {
		return false;
	}
	QString BlockMushroom::getDisplayName() const {
		return "蘑菇";
	}
}