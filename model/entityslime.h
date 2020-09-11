#ifndef ENTITYSLIME_H
#define ENTITISLIME_H
#include "../utils/consts.h"
#include "entityplayerlike.h"
#include <QDataStream>
#include <QString>

namespace parkour {
class EntitySlime : public EntityPlayerLike {
    Q_OBJECT
    const int WAIT_TIMEOUT_MAX = TICKS_PER_SEC * 5, WAIT_TIMEOUT_MIN = TICKS_PER_SEC * 3;
    const double DAMAGE_PER_HIT = 0.1, COLLISION_MULTIPLIER = 0.1;
    int waitTicksLeft;
    void serializeCustomProps(QDataStream& out) const override;
    void deserializeCustomProps(QDataStream& in) override;
    int getSerializationVersion() const override;

public:
    Q_INVOKABLE EntitySlime();

    QString getName() const override;
    QString getResourceLocation() override;
    QVector2D getTextureDimensions() override;
    BoundingBox getBoundingBox() const override;
    double getWalkSpeed() const override;
    double getMass() const override;
    void collide(ICollidable& other, Direction dir) override;
    void update() override;
    QString getDisplayName() const override;
    void damage(double value) override;
    double getMaxHp() const override;
    void jump() override;
};
}
Q_DECLARE_METATYPE(parkour::EntitySlime*)
#endif
