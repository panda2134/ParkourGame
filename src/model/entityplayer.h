#ifndef ENTITY_PLAYER_H
#define ENTITY_PLAYER_H

#include "./entityplayerlike.h"
#include <QVector2D>

namespace parkour {
class EntityPlayer : public EntityPlayerLike {
	Q_OBJECT

	const double DRAG_FACTOR = 1.5;
	const double GROUND_STOP_THRESHOLD = 3;
	double maxHp;
	bool shootFireballs;
	int exp;

	void serializeCustomProps(QDataStream & out) const override;
	void deserializeCustomProps(QDataStream & in) override;
	int getSerializationVersion() const override;
public:
	Q_INVOKABLE EntityPlayer();
    BoundingBox getBoundingBox() const override;
    QString getName() const override;
    QString getResourceLocation() override;
    QVector2D getTextureDimensions() override;
	void update() override;
	void setMaxHp(double value);
	double getMaxHp() const override;
	int getExp() const;
	void increaseExp(int delta);
	bool isAbleToShootFireballs();
	void setShootFireballs(bool value);
};
Q_DECLARE_METATYPE(EntityPlayer*)
}

#endif // ENTITY_PLAYER_H
