#include "blockgrass.h"

namespace parkour {

QString BlockGrass::getName() const {
    return "grass";
}

QString BlockGrass::getResourceLocation() const {
    return ":/assets/blocks/grass.png";
}

QString BlockGrass::getDisplayName() const {
    return "草方块";
}

double BlockGrass::getExplosionResistance() const {
    return 1.0;
}
}
