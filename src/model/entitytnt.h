#ifndef ENTITYTNT_H
#define ENTITYTNT_H

#include "../utils/consts.h"
#include "entity.h"

namespace parkour {
class EntityTNT : public Entity {
public:
    EntityTNT();

    const int TICKS_BEFORE_EXPLOSION = TICKS_PER_SEC * 3;
    int ticksLeft = 0;

public:
    QString getName() const override;
    void update() override;
    void damage(double value) override;
    QString getResourceLocation() override;
    QVector2D getTextureDimensions() override;
    BoundingBox getBoundingBox() const override;
};
}

#endif // ENTITYTNT_H
