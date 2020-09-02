#ifndef ENTITYFIREBALL_H
#define ENTITYFIREBALL_H

#include "../utils/consts.h"
#include "entity.h"
#include <QDataStream>

namespace parkour {

class EntityFireball : public Entity
{
    Q_OBJECT
    double explosionPower;
    int livingTicks;
    const int MAX_AGE = TICKS_PER_SEC * 10;
	virtual void serializeCustomProps(QDataStream & out) const override;
	virtual void deserializeCustomProps(QDataStream & in) override;
	virtual int getSerializationVersion() const override;
public:
	Q_INVOKABLE EntityFireball(double power = .0);
    QString getName() const override;
    bool isAffectedByGravity() const override;
    bool isAffectedByExplosionWave() const override;
    QString getResourceLocation() override;
    QVector2D getTextureDimensions() override;
    BoundingBox getBoundingBox() const override;
    void damage(double value) override;
    bool showDeathAnimation() const override;
    void update() override;
	virtual QString getDisplayName() const override;
	void collide(ICollidable&, Direction) override;
	static void placeFireball(QVector2D position, QVector2D velocity, double power = 7.0);
};
Q_DECLARE_METATYPE(EntityFireball*) 

}
#endif // ENTITYFIREBALL_H
