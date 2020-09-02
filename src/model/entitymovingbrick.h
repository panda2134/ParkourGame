#ifndef ENTITYMOVINGBRICK_H
#define ENTITYMOVINGBRICK_H

#include "./entityplayerlike.h"
#include <QDebug>
#include <QVector2D>
#include <QDataStream>

namespace parkour {

class EntityMovingBrick : public EntityPlayerLike {
	Q_OBJECT

	enum MovingState {
		LEFT, RIGHT, STOP
	};

	MovingState state = MovingState::STOP;
	int ticksLeft = -1;
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
	QString getDisplayName() const override;
	double getMass() const override;
	void collide(ICollidable &other, Direction dir) override;

};
Q_DECLARE_METATYPE(EntityMovingBrick*)
}

#endif // ENTITYMOVINGBRICK_H
