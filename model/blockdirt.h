#ifndef BLOCKDIRT_H
#define BLOCKDIRT_H

#include "block.h"
#include <QObject>

namespace parkour {

class BlockDirt : public Block {
    Q_OBJECT
public:
    BlockDirt() = default;
    QString getResourceLocation() const override;
    QString getName() const override;
    double getExplosionResistance() const override;
    virtual QString getDisplayName() const override;
};
}

#endif // BLOCKDIRT_H
