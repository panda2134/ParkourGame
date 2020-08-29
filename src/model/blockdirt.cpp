#include "blockdirt.h"
#include "world.h"

QString parkour::BlockDirt::getResourceLocation() const {
    return ":/assets/blocks/dirt.png";
}

QString parkour::BlockDirt::getName() const {
    return "dirt";
}

double parkour::BlockDirt::getExplosionResistance() const {
    return 1.0;
}
