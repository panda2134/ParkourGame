#include "blockbedrock.h"

namespace parkour {
QString BlockBedrock::getName() const {
    return "bedrock";
}

QString BlockBedrock::getResourceLocation() const {
    return ":/assets/blocks/bedrock.png";
}

double BlockBedrock::getExplosionResistance() const {
    return 16000000.0;
}
QString BlockBedrock::getDisplayName() const {
    return "基岩";
}
}
