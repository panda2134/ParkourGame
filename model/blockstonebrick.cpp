#include "blockstonebrick.h"

QString parkour::BlockStoneBrick::getName() const {
    return "stone_brick";
}

QString parkour::BlockStoneBrick::getResourceLocation() const {
    return ":/assets/blocks/stone_brick.png";
}

QString parkour::BlockStoneBrick::getDisplayName() const {
    return "石砖";
}

double parkour::BlockStoneBrick::getExplosionResistance() const {
    return 10.0;
}
