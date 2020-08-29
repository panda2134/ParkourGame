#ifndef BLOCKGRASS_H
#define BLOCKGRASS_H

#include "block.h"
#include <QDebug>
#include <QString>

namespace parkour {
class BlockGrass : public Block
{
    Q_OBJECT
public:
    BlockGrass() = default;

    QString getName() const override;
    QString getResourceLocation() const override;

    double getExplosionResistance() const override;
};
}

#endif // BLOCKGRASS_H
