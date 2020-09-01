#include "blockdelegate.h"
#include "entity.h"
#include "registry.h"

namespace parkour {

BlockDelegate::BlockDelegate(QString blockName, QPoint blockPos_)
    : blockPos(blockPos_) {
    this->block = registry::BlockRegistry::instance().getBlockByName(blockName);
}

BoundingBoxWorld BlockDelegate::getBoundingBoxWorld() const {
    QVector2D blockPosVect(blockPos);
	Q_ASSERT(block != nullptr);
    BoundingBox bbox = block->getBoundingBox();
    BoundingBoxWorld ret(blockPosVect, bbox);
    return ret;
}

void BlockDelegate::collide(ICollidable& other, Direction dir) {
	Q_ASSERT(block != nullptr);
	try {
		block->collide(this->blockPos, dynamic_cast<Entity&>(other), dir);
	} catch (std::bad_cast) {
		qDebug() << "WARNING: casting failed";
	}
}

QVector2D BlockDelegate::getVelocity() const {
    // 方块始终静止
    return QVector2D(0, 0);
}

void BlockDelegate::setVelocity(const QVector2D&) {
    // 方块始终静止，不能设置速度
    return;
}

double BlockDelegate::getMass() const {
	return 1e10;
}

QString BlockDelegate::getName() const {
	return block == nullptr ? "air" : block->getName();
}

QString BlockDelegate::getType() const {
	return "block";
}

}
