#include "blockgrass.h"

namespace parkour {

QString BlockGrass::getName() const {
    return "grass";
}

QString BlockGrass::getResourceLocation() const {
    return ":/assets/blocks/grass.png";
}

double BlockGrass::getExplosionResistance() const {
    return 1.0;
}
}
