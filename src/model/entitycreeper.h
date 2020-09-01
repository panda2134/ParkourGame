#ifndef ENTITYCREEPER_H
#define ENTITYCREEPER_H

#include "entityplayerlike.h"
#include "../utils/consts.h"
#include <QDataStream>

namespace parkour {

	class EntityCreeper: public EntityPlayerLike {
		Q_OBJECT
		const double EXPLOSION_THRESHOLD = 4.0;
		const int RANDOM_TICK_LENGTH = TICKS_PER_SEC * 5;
		int randomTicksLeft;
		void serializeCustomProps(QDataStream & out) const override;
		void deserializeCustomProps(QDataStream & in) override;
		int getSerializationVersion() const override;

	public:
		Q_INVOKABLE EntityCreeper();
		QString getName() const override;
		QString getResourceLocation() override;
		QVector2D getTextureDimensions() override;
		BoundingBox getBoundingBox() const override;
		void update() override;
		void collide(ICollidable& other, Direction dir) override;
		double getWalkSpeed() const override;
		bool showDeathAnimation() const override;
	};
	Q_DECLARE_METATYPE(EntityCreeper*)

}

#endif