#include "itemblock.h"
#include "world.h"
#include "blockdelegate.h"
#include "../utils/consts.h"
#include <QtMath>

namespace parkour {
	ItemBlock::ItemBlock(QSharedPointer<Block> block_): block(block_) {
		QImage resource(block->getResourceLocation());
		QRect frame(0, 0, BLOCK_TEXTURE_SIZE, BLOCK_TEXTURE_SIZE);
		texture = resource.copy(frame);
	}
	QString ItemBlock::getDisplayName() {
		return block->getDisplayName();
	}
	const QImage & ItemBlock::getIcon() {
		return texture;
	}
	void ItemBlock::onUse(QVector2D clickPos) {
		if (clickPos.x() < 0 || clickPos.x() >= WORLD_WIDTH || clickPos.y() < 0 || clickPos.y() >= WORLD_HEIGHT) {
			return;
		}
		auto &world = World::instance();
		QPoint blockPos(qFloor(clickPos.x()), qFloor(clickPos.y()));
		if (world.getBlock(blockPos) == "air" && block->canPlaceAt(blockPos)) {
			world.setBlock(blockPos, block->getName());
			auto blockDelegate = BlockDelegate(blockPos);
			for (auto entity : world.getEntities()) {
				if (BoundingBoxWorld::intersect(entity->getBoundingBoxWorld(), blockDelegate.getBoundingBoxWorld())) {
					world.setBlock(blockPos, "air");
					return;
				}
			}
		}
	}
}