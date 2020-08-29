#ifndef ENTITY_PLAYER_LIKE_H
#define ENTITY_PLAYER_LIKE_H

#include "../utils/direction.h"
#include "entity.h"
#include "icollidable.h"
#include <QtGlobal>

namespace parkour {
class EntityPlayerLike : public Entity {
    Q_OBJECT

    bool sneak;

public:
    EntityPlayerLike();
    /**
     * @brief collide 设置摔落伤害等 子类若重载，则必须首先调用此方法！
     */
    void collide(const ICollidable&, Direction) override;

    virtual void jump();
    virtual void goLeft();
    virtual void goRight();
    bool isSneak() const;
    bool setSneak(bool value);
};
}

#endif // ENTITY_PLAYER_LIKE_H
