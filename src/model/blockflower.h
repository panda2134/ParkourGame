#ifndef BLOCKFLOWER_H
#define BLOCKFLOWER_H

#include "block.h"

namespace parkour {
	class BlockFlower: public Block {
		const double HP_BONUS = 10.0;
	public:
		QString getName() const override;
		QString getResourceLocation() const override;
		double getExplosionResistance() const override;

		BoundingBox getBoundingBox() const override;
		void collide(QPoint blockPos, Entity &entity, Direction dir);

		virtual bool canPlaceAt(QPoint) const override;
		virtual bool isSolid() const;

		virtual QString getDisplayName() const override;
	};
}
#endif