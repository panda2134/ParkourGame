#include "world.h"
#include "itemmovingbrick.h"
#include "entitymovingbrick.h"
#include <QSharedPointer>

namespace parkour {
	ItemMovingBrick::ItemMovingBrick() {
		icon.load(":/assets/entities/moving_brick.png");
	}
	QString ItemMovingBrick::getDisplayName() {
		return QString("设置可移动方块 - 选择%1").arg(QString(firstPointSelected ? "终点" : "起点"));
	}
	const QImage & ItemMovingBrick::getIcon() {
		return icon;
	}
	void ItemMovingBrick::onUse(QVector2D clickPos) {
		if (!firstPointSelected) {
			firstPoint = clickPos;
		} else {
			auto movingBrick = QSharedPointer<EntityMovingBrick>::create();
			movingBrick->setEnds(firstPoint, clickPos);
			movingBrick->placeBoundingBoxAt(firstPoint);
			World::instance().addEntity(movingBrick);
		}
		firstPointSelected ^= 1;
	}
}