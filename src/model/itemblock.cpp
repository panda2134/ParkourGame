#include "itemblock.h"
#include "world.h"
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
	void ItemBlock::onUse(QVector2D blockPos) {
		auto &world = World::instance();
		qDebug("using itemblock");
		QPoint point(qFloor(blockPos.x()), qFloor(blockPos.y()));
		if (world.getBlock(point) == "air") {
			qDebug() << block->getName();
			world.setBlock(point, block->getName());
		}
	}
}