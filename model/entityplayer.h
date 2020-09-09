#ifndef ENTITY_PLAYER_H
#define ENTITY_PLAYER_H

#include "entityplayerlike.h"
#include "../utils/consts.h"
#include "item.h"
#include <QList>
#include <QVector2D>
#include <QSharedPointer>

namespace parkour {

	class PlayerController;

class EntityPlayer : public EntityPlayerLike {
	Q_OBJECT

	const double DRAG_FACTOR = 3;
	const double GROUND_STOP_THRESHOLD = 3;
	double maxHp;
	bool shootFireballs;
	int exp;
	QSharedPointer<Item> inventory[PLAYER_INVENTORY_SLOT_COUNT]; // 无需序列化

	void serializeCustomProps(QDataStream & out) const override;
	void deserializeCustomProps(QDataStream & in) override;
	int getSerializationVersion() const override;

	double getWalkSpeed() const override;
	
	void loadInventory();
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
	QString getDisplayName() const override;
	void setShootFireballs(bool value);
	void damage(double value) override;
	QSharedPointer<Item>* getInventory();
};
}
Q_DECLARE_METATYPE(parkour::EntityPlayer*)

#endif // ENTITY_PLAYER_H
