#ifndef ENTITYSLIME_H
#define ENTITISLIME_H
#include "entityplayerlike.h"
#include "../utils/consts.h"

namespace parkour {
	class EntitySlime: public EntityPlayerLike {
		const int WAIT_TIMEOUT_MAX = TICKS_PER_SEC * 5, WAIT_TIMEOUT_MIN = TICKS_PER_SEC * 3;
		const double DAMAGE_PER_HIT = 0.1, COLLISION_MULTIPLIER = 0.1;
		int waitTicksLeft;

	public:
		EntitySlime();

		QString getName() const override;
		QString getResourceLocation() override;
		QVector2D getTextureDimensions() override;
		BoundingBox getBoundingBox() const override;
		double getWalkSpeed() const override;
		double getMass() const override;
		void collide(ICollidable &other, Direction dir) override;
		void update() override;
	};
}
#endif
