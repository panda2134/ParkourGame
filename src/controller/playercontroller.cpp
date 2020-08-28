#include "playercontroller.h"

namespace parkour {
bool PlayerController::isAlive() const {
    return alive;
}

QSharedPointer<PlayerEntity> PlayerController::getPlayer() {
    for (const auto& entity : World::instance().getEntities()) {
        auto casted = dynamic_cast<PlayerEntity*>(entity.data());
        if (casted != nullptr) {
            return entity.objectCast<PlayerEntity>();
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

PlayerController::PlayerController()
    : alive(false)
    , readyJump(false)
    , goingLeft(false)
    , goingRight(false) {
}

void PlayerController::tick() {
    World& world = World::instance();
    if (!world.isReady()) {
        return;
    }

    //    qDebug() << "start ticking in player controller";

    QSharedPointer<PlayerEntity> player;

    alive = false;

    if ((player = getPlayer()) != nullptr) {
        alive = true;
    }

    if (!alive) {
        for (const auto& dyingEntity : world.getDyingEntities().keys()) {
            auto casted = dynamic_cast<PlayerEntity*>(dyingEntity.data());
            if (casted != nullptr) {
                //                qDebug() << "player is dying";
                player = dyingEntity.objectCast<PlayerEntity>();
            }
        }

        if (player == nullptr) {
            // 玩家已经死亡，在重生点重生
            qDebug() << "respawn!";
            player = QSharedPointer<PlayerEntity>::create();
            player->setPosition(world.getSpawnPoint());
            world.addEntity(player);
            alive = true;
        }
    }

    if (alive) {
        if (readyJump) {
            //            qDebug() << "jumping";
            player->jump();
        }

        if (goingLeft ^ goingRight) {
            if (goingLeft) {
                //                qDebug() << "going left";
                player->goLeft();
            } else {
                //                qDebug() << "going right";
                player->goRight();
            }
        } else {
            //            qDebug() << "set to stop";
            player->setVelocity({ 0, player->getVelocity().y() });
        }
    }
}

}
