#include "blockflower.h"
#include "entityplayer.h"
#include "world.h"
#include "registry.h"

namespace parkour {
	QString BlockFlower::getName() const {
		return "flower";
	}
	QString BlockFlower::getResourceLocation() const {
		return ":/assets/blocks/flower.png";
	}
	double BlockFlower::getExplosionResistance() const {
		return 0.1;
	}
	BoundingBox BlockFlower::getBoundingBox() const {
		return BoundingBox{ {6.0f / 16, 5.0f / 16}, {4.0f / 16, 11.0f / 16} };
	}
	void BlockFlower::collide(QPoint blockPos, Entity & entity, Direction dir) {
		if (entity.getName() == "player") {
			auto& player = static_cast<EntityPlayer&>(entity);
			player.setMaxHp(player.getMaxHp() + HP_BONUS);
			player.regenerate();
			player.setShootFireballs(true);
			World::instance().setBlock(blockPos, "air");
		}
	}
	bool BlockFlower::canPlaceAt(QPoint blockPos) const {
		auto block = registry::BlockRegistry::instance().getBlockByName(World::instance().getBlock(blockPos + QPoint(0, 1)));
		return block != nullptr && (block->getName() == "dirt" || block->getName() == "grass");
	}
	bool BlockFlower::isSolid() const {
		return false;
	}
	QString BlockFlower::getDisplayName() const {
		return "花";
	}
}