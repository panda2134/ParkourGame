#ifndef PLAYERENTITY_H
#define PLAYERENTITY_H

#include "playerlikeentity.h"
#include <QVector2D>

namespace parkour {

class PlayerEntity : public PlayerLikeEntity {
    Q_OBJECT
public:
    PlayerEntity();

    bool sneak;

    // ICollidable interface
public:
    BoundingBox getBoundingBox() const override;

    // Entity interface
public:
    QString getName() const override;
    QString getResourceLocation() override;
    QVector2D getTextureDimensions() override;
    bool isSneak() const;
    void setSneak(bool value);
};
}

#endif // PLAYERENTITY_H
