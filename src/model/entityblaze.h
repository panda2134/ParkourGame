#ifndef ENTITYBLAZE_H
#define ENTITYBLAZE_H

#include "../utils/consts.h"
#include "entity.h"
#include <QPair>
#include <QQueue>
#include <QDataStream>

namespace parkour {

class EntityBlaze : public Entity
{
    Q_OBJECT

    enum BlazeState {
        LOWEST,
        UP,
        HIGHEST,
        DOWN
    };

    const int BLAZE_FRAME_LENGTH = TICKS_PER_SEC * 0.06, SHOOT_INTERVAL = TICKS_PER_SEC * 0.2, COOLDOWN_TICKS = TICKS_PER_SEC * 3;
    const double CHANGE_STATE_POSSIBILITY = 1 / (30.0 * TICKS_PER_SEC);
    const double ATTACK_POSSIBILITY = 1 / (5.0 * TICKS_PER_SEC);
    const float VERTICAL_VELOCITY = 2.0f, FIREBALL_VELOCITY = 14.0f;
    const int FIREBALLS_PER_ATTACK = 3;
    int64_t livingTicks, cooldown;
    BlazeState state;
    QQueue<QPair<int, QVector2D>> attackQueue;
    void pushAttackQueue(QSharedPointer<Entity> entity);

	void serializeCustomProps(QDataStream& out) const override;
	void deserializeCustomProps(QDataStream& in) override;

	virtual int getSerializationVersion() const override;

	bool blocked(float offsetY);

public:
	Q_INVOKABLE EntityBlaze();

    // ICollidable interface
public:
    void collide(ICollidable& other, Direction dir) override;

    // Entity interface
public:
    QString getName() const override;
    void update() override;
    bool isAffectedByGravity() const override;
    QString getResourceLocation() override;
    QVector2D getTextureDimensions() override;
    BoundingBox getBoundingBox() const override;
	double getMass() const override;
	void damage(double val) override;
	virtual QString getDisplayName() const override;
};
Q_DECLARE_METATYPE(EntityBlaze*)

}

#endif // ENTITYBLAZE_H
