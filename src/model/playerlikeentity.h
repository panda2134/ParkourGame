#ifndef PLAYERLIKEENTITY_H
#define PLAYERLIKEENTITY_H

#include "../utils/consts.h"
#include "entity.h"
#include <QtGlobal>

namespace parkour {
class PlayerLikeEntity : public Entity
{
    Q_OBJECT
public:
    PlayerLikeEntity() = default;
    /**
     * @brief collide 设置摔落伤害等 子类若重载，则必须首先调用此方法！
     */
    void collide(const ICollidable&, Direction) override;

    void jump();
};
}

#endif // PLAYERLIKEENTITY_H
