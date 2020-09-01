#ifndef ENTITYMOVINGBRICK_H
#define ENTITYMOVINGBRICK_H

#include "./entityplayerlike.h"
#include <QDebug>
#include <QVector2D>
#include <QDataStream>

namespace parkour {

class EntityMovingBrick : public EntityPlayerLike {
	Q_OBJECT

	int ticks = 0;
	double getWalkSpeed() const override;
	void serializeCustomProps(QDataStream & out) const override;
	void deserializeCustomProps(QDataStream & in) override;
	int getSerializationVersion() const override;

public:
	Q_INVOKABLE EntityMovingBrick() = default;
    QString getName() const override;
    QString getResourceLocation() override;
    QVector2D getTextureDimensions() override;

    QVector2D offset, dimensions;

    BoundingBox getBoundingBox() const override;

    void update() override;
	bool isAffectedByGravity() const override;
	double getMass() const override;

};
Q_DECLARE_METATYPE(EntityMovingBrick*)
}

#endif // ENTITYMOVINGBRICK_H
