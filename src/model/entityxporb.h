#ifndef ENTITYXPORB_H
#define ENTITYXPORB_H

#include "icollidable.h"
#include "../utils/direction.h"
#include "entity.h"

namespace parkour {
	class EntityXpOrb : public Entity {
		Q_OBJECT

		int xp, tick;

	public:
		EntityXpOrb();

		virtual void serializeCustomProps(QDataStream & out) const override;
		virtual void deserializeCustomProps(QDataStream & in) override;
		virtual int getSerializationVersion() const override;
		virtual QString getName() const override;
		virtual QString getResourceLocation() override;
		virtual QVector2D getTextureDimensions() override;
		virtual BoundingBox getBoundingBox() const override;
		virtual QRect getTextureRenderRect() const override;
		virtual void update() override;
		virtual void collide(ICollidable &other, Direction dir) override;
		virtual bool showDeathAnimation() const override;
		virtual bool isAffectedByExplosionWave() const override;
		virtual void damage(double val) override;
		virtual double getMass() const override;
		void setXp(int val);
		QString getDisplayName() const override;
		static void dropXpOrbs(QVector2D position, int xp);
	};
}

#endif