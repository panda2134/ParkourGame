#ifndef BLOCKEXPBOTTLE_H
#define BLOCKEXPBOTTLE_H

#include "block.h"

namespace parkour {
class BlockExpBottle : public Block {
    Q_OBJECT

public:
    virtual QString getName() const override;
    virtual QString getResourceLocation() const override;
    virtual double getExplosionResistance() const override;
    virtual QString getDisplayName() const override;
    virtual void collide(QPoint, Entity&, Direction) override;
    virtual void onStand(QPoint blockPos, Entity& entity) override;
};
}

#endif