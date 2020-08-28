#ifndef PLAYERCONTROLLER_H
#define PLAYERCONTROLLER_H

#include "../model/playerentity.h"
#include "../model/world.h"
#include "../utils/askeyvaluerange.h"
#include <QObject>
#include <QWeakPointer>

namespace parkour {
class PlayerController : public QObject
{
    Q_OBJECT

    bool alive, readyJump, goingLeft, goingRight;

public:
    explicit PlayerController();

    void tick();
    bool isAlive() const;
    /**
     * @brief getPlayer
     * @return 在存活且非濒临死亡的实体中寻找玩家，若无则返回nullptr
     */
    QSharedPointer<PlayerEntity> getPlayer();

public slots:
    void setReadyJump(bool value);
    void setGoingLeft(bool value);
    void setGoingRight(bool value);
};
}

#endif // PLAYERCONTROLLER_H
