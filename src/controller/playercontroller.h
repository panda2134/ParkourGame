#ifndef PLAYERCONTROLLER_H
#define PLAYERCONTROLLER_H

#include "../model/entityplayer.h"
#include <QObject>
#include <QSharedPointer>

namespace parkour {
class PlayerController : public QObject
{
	Q_OBJECT

	const double FIREBALL_VELOCITY = 18.0, FIREBALL_POWER = 3.0;

    /**
     * @brief alive 是否存活
     */
    bool alive;
    /**
     * @brief readyJump 是否准备一次跳跃（即是否跳跃键被按住）
     */
    bool readyJump;
    /**
     * @brief goingLeft 是否往左（即是否向左走键被按住）
     */
    bool goingLeft;
    /**
     * @brief goingRight 是否往右（即是否向右走键被按住）
     */
    bool goingRight;
    /**
     * @brief sneakingExpected 是否期待下蹲状态（即是否下蹲键被按住）
     */
    bool sneakingExpected;

public:
    explicit PlayerController();

    void tick();
	void shootFireballAt(QVector2D target);
    bool isAlive() const;
    /**
     * @brief getPlayer
     * @return 在存活且非濒临死亡的实体中寻找玩家，若无则返回nullptr
     */
    QSharedPointer<EntityPlayer> getPlayer();

public slots:
    void setReadyJump(bool value);
    void setGoingLeft(bool value);
    void setGoingRight(bool value);
    void setSneakingExpected(bool value);
};
}

#endif // PLAYERCONTROLLER_H
