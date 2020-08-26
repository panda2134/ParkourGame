#include "world.h"

namespace parkour {

bool World::isReady() const {
    return ready;
}

void World::setReady(bool value) {
    ready = value;
}

void World::setBlock(BlockPosType blockPos, QString blockName) {
    if (!ready) {
        throw std::exception("not ready");
    }
    blocks[blockPos] = blockName;
}

QString World::getBlock(BlockPosType blockPos) {
    if (!ready) {
        throw std::exception("not ready");
    }

    if (blocks.count(blockPos)) {
        return blocks[blockPos];
    } else {
        return "air";
    }
}

}
