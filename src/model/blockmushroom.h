#ifndef BLOCKMUSHROOM_H
#define BLOCKMUSHROOM_H

#include "block.h"

namespace parkour {
	class BlockMushroom: public Block {
		const double HP_BOOST = 20.0;
	public:
		QString getName() const override;
		QString getResourceLocation() const override;
		double getExplosionResistance() const override;
		void collide(QPoint blockPos, Entity& entity, Direction dir) override;
		BoundingBox getBoundingBox() const override;
	};
}

#endif
