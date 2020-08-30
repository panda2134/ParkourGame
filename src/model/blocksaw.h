#ifndef BLOCKSAW_H
#define BLOCKSAW_H

#include "block.h"

namespace parkour {
	class BlockSaw : public Block {
		Q_OBJECT

		const double DAMAGE_PER_COLLISION = 0.4;
		const double BOUNCE_AWAY_SPEED = 6.0;

	public:
		BlockSaw();

		virtual QString getName() const override;
		virtual QString getResourceLocation() const override;
		virtual double getExplosionResistance() const override;
		
		virtual void collide(QPoint, Entity&, Direction) override;
	};

}
#endif