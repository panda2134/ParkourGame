#include "blockgrass.h"

namespace parkour {
BlockGrass::BlockGrass()
{

}

QString BlockGrass::getName() const {
    return "grass";
}

QString BlockGrass::getResourceLocation() const {
    return ":/assets/blocks/grass.png";
}

void BlockGrass::collide(QVector2D blockPos, const Entity& entity) {
    qDebug() << "collide at block:" << blockPos << ',' << "entity name = " << entity.getName();
}

}
