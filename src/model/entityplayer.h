#ifndef ENTITY_PLAYER_H
#define ENTITY_PLAYER_H

#include "./entityplayerlike.h"
#include <QVector2D>

namespace parkour {
class EntityPlayer : public EntityPlayerLike {
	Q_OBJECT

	const double DRAG_FACTOR = 5;
	const double GROUND_STOP_THRESHOLD = 3;
	double maxHp;
	bool shootFireballs;
public:
    EntityPlayer();
    BoundingBox getBoundingBox() const override;
    QString getName() const override;
    QString getResourceLocation() override;
    QVector2D getTextureDimensions() override;
	void update() override;
	void setMaxHp(double value);
	double getMaxHp() const override;
	bool isAbleToShootFireballs();
	void setShootFireballs(bool value);
};
}

#endif // ENTITY_PLAYER_H
