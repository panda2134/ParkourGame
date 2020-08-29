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
    BoundingBox bbox = block->getBoundingBox();
    BoundingBoxWorld ret(blockPosVect, bbox);
    return ret;
}

void BlockDelegate::collide(const ICollidable& other, Direction dir) {
    block->collide(this->blockPos, dynamic_cast<const Entity&>(other), dir);
}

QVector2D BlockDelegate::getVelocity() const {
    // 方块始终静止
    return QVector2D(0, 0);
}

void BlockDelegate::setVelocity(const QVector2D&) {
    // 方块始终静止，不能设置速度
    return;
}
}
