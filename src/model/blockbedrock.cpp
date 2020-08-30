#include "blockbedrock.h"

QString parkour::BlockBedrock::getName() const {
	return "bedrock";
}

QString parkour::BlockBedrock::getResourceLocation() const {
	return ":/assets/blocks/bedrock.png";
}

double parkour::BlockBedrock::getExplosionResistance() const {
	return 16000000.0;
}
