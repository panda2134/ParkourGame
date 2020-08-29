#include "playercontroller.h"
#include "../model/world.h"
#include "../utils/askeyvaluerange.h"

namespace parkour {
bool PlayerController::isAlive() const {
    return alive;
}

QSharedPointer<EntityPlayer> PlayerController::getPlayer() {
    for (const auto& entity : World::instance().getEntities()) {
        auto casted = dynamic_cast<EntityPlayer*>(entity.data());
        if (casted != nullptr) {
            return entity.objectCast<EntityPlayer>();
        }
    }
    return nullptr;
}

void PlayerController::setReadyJump(bool value) {
    this->readyJump = value;
}

void PlayerController::setGoingLeft(bool value) {
    goingLeft = value;
}

void PlayerController::setGoingRight(bool value) {
    goingRight = value;
}

void PlayerController::setSneakingExpected(bool value) {
    sneakingExpected = value;
}

PlayerController::PlayerController()
    : alive(false)
    , readyJump(false)
    , goingLeft(false)
    , goingRight(false)
    , sneakingExpected(false) {
}

void PlayerController::tick() {
    World& world = World::instance();
    if (!world.isReady()) {
        return;
    }

    //    qDebug() << "start ticking in player controller";

    QSharedPointer<EntityPlayer> player;

    alive = false;

    if ((player = getPlayer()) != nullptr) {
        alive = true;
    }

    if (!alive) {
        for (const auto& dyingEntity : world.getDyingEntities().keys()) {
            auto casted = dynamic_cast<EntityPlayer*>(dyingEntity.data());
            if (casted != nullptr) {
                //                qDebug() << "player is dying";
                player = dyingEntity.objectCast<EntityPlayer>();
            }
        }

        if (player == nullptr) {
            // 玩家已经死亡，在重生点重生
            qDebug() << "respawn!";
            player = QSharedPointer<EntityPlayer>::create();
            player->setPosition(world.getSpawnPoint());
            world.addEntity(player);
            alive = true;
        }
    }

    if (alive) {
        if (readyJump) {
            player->jump();
        }

        player->setSneak(sneakingExpected);

        if (goingLeft ^ goingRight) {
            if (goingLeft) {
                player->goLeft();
            } else {
                player->goRight();
            }
        } else {
            player->setVelocity({ 0, player->getVelocity().y() });
        }
    }
}

}