#include "entityplayer.h"

namespace parkour {

EntityPlayer::EntityPlayer()
    : EntityPlayerLike() {
    this->setHp(20.0);
}

BoundingBox EntityPlayer::getBoundingBox() const {
    if (isSneak()) {
        return BoundingBox { QVector2D(0.25, 1.05), QVector2D(0.5, 0.9) };
    } else {
        return BoundingBox { QVector2D(0.25, 0.05), QVector2D(0.5, 1.9) };
    }
}

QString EntityPlayer::getName() const {
    return "player";
}

QString EntityPlayer::getResourceLocation() {
    if (!isSneak()) {
        return ":/assets/entities/player.png";
    } else {
        return ":/assets/entities/player-sneak.png";
    }
}

QVector2D EntityPlayer::getTextureDimensions() {
    return QVector2D(1, 2);
}

}
