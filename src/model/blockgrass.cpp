#include "blockgrass.h"

namespace parkour {

QString BlockGrass::getName() const {
    return "grass";
}

QString BlockGrass::getResourceLocation() const {
    return ":/assets/blocks/grass.png";
}

void BlockGrass::collide(QPoint blockPos, const Entity& entity, Direction dir) {
    qDebug() << "grass: collide at block:" << blockPos << ',' << "entity name = " << entity.getName() << ',' << "dir = " << dir;
}

double BlockGrass::getExplosionResistance() const {
    return 1.0;
}
}
