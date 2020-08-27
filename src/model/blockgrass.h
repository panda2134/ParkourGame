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

    void collide(QPoint blockPos, const Entity& entity, Direction) override;
};
}

#endif // BLOCKGRASS_H
