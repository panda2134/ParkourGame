#include "blockdelegate.h"

namespace parkour {

BlockDelegate::BlockDelegate(QString blockName, BlockPosType blockPos_)
    : blockPos(blockPos_) {
    this->block = registry::getBlockByName(blockName);
}

BoundingBoxWorld BlockDelegate::getBoundingBoxWorld() const {
    QVector2D blockPosVect(blockPos.first, blockPos.second);
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

}
