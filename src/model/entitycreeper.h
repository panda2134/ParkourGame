#ifndef ENTITYCREEPER_H
#define ENTITYCREEPER_H

#include "entityplayerlike.h"
#include "../utils/consts.h"

namespace parkour {

	class EntityCreeper: public EntityPlayerLike {
		const double EXPLOSION_THRESHOLD = 4.0;
		const int RANDOM_TICK_LENGTH = TICKS_PER_SEC * 5;
		int randomTicksLeft;

	public:
		EntityCreeper();
		virtual QString getName() const override;
		virtual QString getResourceLocation() override;
		virtual QVector2D getTextureDimensions() override;
		virtual BoundingBox getBoundingBox() const override;
		void update() override;
		void collide(ICollidable& other, Direction dir) override;
		double getWalkSpeed() const override;
		bool showDeathAnimation() const override;
	};

}

#endif