#ifndef ENTITYMOVINGBRICK_H
#define ENTITYMOVINGBRICK_H

#include "./entity.h"
#include <QDebug>
#include <QVector2D>
#include <QDataStream>

namespace parkour {

class EntityMovingBrick : public Entity {
	Q_OBJECT

	enum MovingState {
		FORWARD, BACKWARD, STOP
	};

	const double movingVelocity = 2.0;

	MovingState state = MovingState::STOP;
	QVector2D start, end;
	int ticksLeft = -1;
	void serializeCustomProps(QDataStream & out) const override;
	void deserializeCustomProps(QDataStream & in) override;
	int getSerializationVersion() const override;
public:
	Q_INVOKABLE EntityMovingBrick();
    QString getName() const override;
    QString getResourceLocation() override;
    QVector2D getTextureDimensions() override;
    QVector2D offset, dimensions;
	void setEnds(const QVector2D &, const QVector2D &);

    BoundingBox getBoundingBox() const override;

    void update() override;
	bool isAffectedByGravity() const override;
	QString getDisplayName() const override;
	double getMass() const override;

	void damage(double val) override;
	bool showDeathAnimationAndInfo() const override;
};
}
Q_DECLARE_METATYPE(parkour::EntityMovingBrick*)

#endif // ENTITYMOVINGBRICK_H
