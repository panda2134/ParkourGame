#ifndef BLOCKTNT_H
#define BLOCKTNT_H

#include "block.h"

namespace parkour {
class BlockTNT : public Block
{
    Q_OBJECT
public:
    BlockTNT();

    // Block interface
public:
    QString getName() const override;
    QString getResourceLocation() const override;
	virtual QString getDisplayName() const override;
	double getExplosionResistance() const override;
    void onExplosion(QPoint blockPos, double power) override;
    void collide(QPoint, Entity&, Direction) override;
	void onStand(QPoint, Entity&) override;
};
}

#endif // BLOCKTNT_H
