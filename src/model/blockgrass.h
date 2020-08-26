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
    BlockGrass();

    QString getName() const override;
    QString getResourceLocation() const override;

    void collide(QVector2D blockPos, const Entity& entity) override;
};
}

#endif // BLOCKGRASS_H
