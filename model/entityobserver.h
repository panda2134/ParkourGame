#ifndef ENTITYOBSERVER_H
#define ENTITYOBSERVER_H

#include "entity.h"

namespace parkour {
	class EntityObserver : public Entity {
		Q_OBJECT
		const int IGNITE_RANGE = 2;
		const double DETECT_RANGE = 5.0;

		bool playerFound = false;
	public:
		Q_INVOKABLE EntityObserver() = default;
		virtual void serializeCustomProps(QDataStream & out) const override;
		virtual void deserializeCustomProps(QDataStream & in) override;
		virtual int getSerializationVersion() const override;
		virtual QString getName() const override;
		virtual QString getResourceLocation() override;
		virtual QVector2D getTextureDimensions() override;
		virtual BoundingBox getBoundingBox() const override;
		virtual QString getDisplayName() const override;
		virtual QRect getTextureRenderRect() const override;
		virtual void update() override;
		virtual bool showDeathAnimationAndInfo() const;
		virtual double getMass() const;
        };
}

Q_DECLARE_METATYPE(parkour::EntityObserver*)
#endif
