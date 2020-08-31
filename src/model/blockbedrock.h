#ifndef BLOCKBEDROCK_H
#define BLOCKBEDROCK_H
#include "block.h"
namespace parkour {
	class BlockBedrock : public Block {
		Q_OBJECT

	public:
		virtual QString getName() const override;
		virtual QString getResourceLocation() const override;
                virtual double getExplosionResistance() const override;
        };
}
#endif

