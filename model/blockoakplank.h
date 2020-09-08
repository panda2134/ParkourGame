#ifndef BLOCKOAKPLANK_H
#define BLOCKOAKPLANK_H
#include "block.h"

namespace parkour {
	class BlockOakPlank : public Block {
	public:
		virtual QString getName() const override;
		virtual QString getResourceLocation() const override;
		virtual double getExplosionResistance() const override;
		virtual QString getDisplayName() const override;
	};
}

#endif