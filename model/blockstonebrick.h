#ifndef BLOCKSTONEBRICK_H
#define BLOCKSTONEBRICK_H

#include "block.h"

namespace parkour {
class BlockStoneBrick : public Block {
public:
    virtual QString getName() const override;
    virtual QString getResourceLocation() const override;
    virtual QString getDisplayName() const override;
    virtual double getExplosionResistance() const override;
};
}

#endif