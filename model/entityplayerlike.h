#ifndef ENTITY_PLAYER_LIKE_H
#define ENTITY_PLAYER_LIKE_H

#include "../utils/direction.h"
#include "entity.h"
#include "icollidable.h"
#include <QtGlobal>

namespace parkour {
class EntityPlayerLike : public Entity {
    Q_OBJECT

    /**
     * @brief sneak 关于下蹲状态，要求下蹲碰撞箱是平常碰撞箱的子集。
     */
    bool sneak;

	virtual double getWalkSpeed() const;

public:
    EntityPlayerLike();
    /**
     * @brief collide 设置摔落伤害等 子类若重载，则必须首先调用此方法！
     */
    void collide(ICollidable&, Direction) override;

    virtual void jump();
    virtual void goLeft();
    virtual void goRight();
    bool isSneak() const;
    bool setSneak(bool value);
};
}

#endif // ENTITY_PLAYER_LIKE_H
