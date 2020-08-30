#ifndef ENTITYMOVINGBRICK_H
#define ENTITYMOVINGBRICK_H

#include "./entityplayerlike.h"
#include <QDebug>
#include <QVector2D>

namespace parkour {

class EntityMovingBrick : public EntityPlayerLike {
	Q_OBJECT

	double getWalkSpeed() const override;

public:
    QString getName() const override;
    QString getResourceLocation() override;
    QVector2D getTextureDimensions() override;
    EntityMovingBrick() = default;

    QVector2D offset, dimensions;

    BoundingBox getBoundingBox() const override;

    void update() override;
	bool isAffectedByGravity() const override;
	double getMass() const override;
};
}

#endif // ENTITYMOVINGBRICK_H
