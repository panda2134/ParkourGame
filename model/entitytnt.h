#ifndef ENTITYTNT_H
#define ENTITYTNT_H

#include "../utils/consts.h"
#include "entity.h"

namespace parkour {
class EntityTNT : public Entity {
	Q_OBJECT
	void serializeCustomProps(QDataStream & out) const override;
	void deserializeCustomProps(QDataStream & in) override;
	int getSerializationVersion() const override;

public:
	Q_INVOKABLE EntityTNT();
    const int TICKS_BEFORE_EXPLOSION = TICKS_PER_SEC * 3;
    const double DRAG_FACTOR = 0.2;
    int ticksLeft = 0;

    QString getName() const override;
    void update() override;
    void damage(double value) override;
    QString getResourceLocation() override;
    QVector2D getTextureDimensions() override;
    BoundingBox getBoundingBox() const override;
	QString getDisplayName() const override;
	bool showDeathAnimationAndInfo() const override;
};
}
Q_DECLARE_METATYPE(parkour::EntityTNT*)

#endif // ENTITYTNT_H
