#include "playercontroller.h"
#include "../model/world.h"
#include "../model/registry.h"
#include "../model/entityfireball.h"
#include "../utils/askeyvaluerange.h"

namespace parkour {
bool PlayerController::isAlive() const {
    return getPlayer() != nullptr;
}

QSharedPointer<EntityPlayer> PlayerController::getPlayer() const {
    for (const auto& entity : World::instance().getEntities()) {
        if (entity->getName() == "player") {
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
    : readyJump(false)
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

    QSharedPointer<EntityPlayer> player = getPlayer();
	bool dying = false;

	if (player == nullptr) {
		for (const auto& dyingEntity : world.getDyingEntities().keys()) {
			if (dyingEntity->getName() == "player") {
				dying = true;
				player = dyingEntity.objectCast<EntityPlayer>();
				break;
			}
		}
	}

	if (!dying) {
		if (player == nullptr) {
			// 玩家已经死亡，在重生点重生
			player = QSharedPointer<EntityPlayer>::create();
			player->setPosition(world.getSpawnPoint());
			world.addEntity(player);
		}

		player = getPlayer();

		if (player != nullptr) {
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
			}
		}
	}
}

void PlayerController::shootFireballAt(QVector2D target) {
	auto player = getPlayer();
	if (player == nullptr || !player->isAbleToShootFireballs()) {
		return;
	}
	auto bbox = player->getBoundingBoxWorld();
	auto position = bbox.getCenter();
	auto norm = QVector2D(target - position).normalized();
	EntityFireball::placeFireball(position + 2 * norm, norm * FIREBALL_VELOCITY, FIREBALL_POWER);
}

}
