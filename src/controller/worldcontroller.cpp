#include "worldcontroller.h"
#include "../model/blockdelegate.h"
#include "../model/entityplayer.h"
#include "../model/registry.h"
#include "../model/testentity.h"
#include "../model/world.h"
#include "../utils/consts.h"
#include "../utils/geometryhelper.h"

namespace parkour {

QSharedPointer<PlayerController> WorldController::getPlayerController() const
{
    return playerController;
}

WorldController::WorldController()
    : playerController(QSharedPointer<PlayerController>::create()) {
}

void WorldController::loadTestWorld() const {
    auto& world = World::instance();
    if (world.isReady()) {
        throw std::exception("test world loading failed: world is already ready");
    }
    qDebug() << "setting block";
    for (int i = 0; i <= 100; i++) {
        world.setBlock(QPoint(i, 15), "grass");
    }
    for (int i = 15; i <= 19; i++) {
        world.setBlock(QPoint(i, 13), "dirt");
    }
    for (int i = 21; i <= 23; i++) {
        world.setBlock(QPoint(i, 11), "dirt");
    }
    for (int i = 25; i <= 26; i++) {
        world.setBlock(QPoint(i, 9), "tnt");
    }
    for (int i = 28; i <= 33; i++) {
        world.setBlock(QPoint(i, 9 + i - 28), "dirt");
    }

    qDebug() << "loading some entities";

    //    auto e1 = QSharedPointer<TestEntity>::create();
    //    e1->placeBoundingBoxAt(QVector2D(9, 8));
    //    world.addEntity(e1);

    qDebug() << "loading player";

    world.setSpawnPoint({ 7, 13 });

    qDebug() << "test world loaded";
    world.setReady(true);
}

void WorldController::unloadWorld() const {
    auto& world = World::instance();
    if (!world.isReady()) {
        throw std::exception("world not ready, cannot unload");
    }
    world.entities.clear();
    world.dyingEntities.clear();
    world.blocks.clear();
    world.setReady(false);
}

void WorldController::explode(QPoint center, double power) const {
    if (power < 0) {
        return;
    }
    auto& world = World::instance();
    if (!world.isReady()) {
        throw std::exception("world not ready");
    }
    // damage blocks
    double radius = power * EXPLOSION_RADIUS_MULTIPLIER;
    for (int i = center.x() - radius; i <= center.x() + radius; i++) {
        for (int j = center.y() - radius; j <= center.y() + radius; j++) {
            auto distance = QVector2D(center - QPoint(i, j)).length();
            auto block = registry::BlockRegistry::instance().getBlockByName(world.getBlock({ i, j }));
            if (block != nullptr && geometry::compareDoubles(radius - distance, block->getExplosionResistance()) >= 0) {
                block->onExplosion({ i, j }, radius - distance);
                world.setBlock({ i, j }, "air");
            }
        }
    }
    for (auto& entity : world.getEntities()) {
        auto distance = QVector2D(entity->getPosition().toPointF() - center).length();
        if (geometry::compareDoubles(distance, radius) <= 0) {
            auto damage = (power * EXPLOSION_DAMAGE_MULTIPLIER / (radius * radius)) * (distance - radius) * (distance - radius);
            entity->damage(damage);
        }
    }
}

void WorldController::tick() const {
    //    qDebug() << "ticking LocalWorldController";
    auto& world = World::instance();

    if (!world.isReady()) {
        throw "ticking in a non-ready world";
    }

    ++world.ticksFromBirth;

    //    qDebug() << "ticksFromBirth = " << world.ticksFromBirth;

    // 0. 维护将死亡实体列表，并把entities中将死亡的实体加入列表
    //    qDebug() << "0. calculating dying entities";
    QMutableHashIterator<QSharedPointer<Entity>, int> itDying(world.dyingEntities);
    while (itDying.hasNext()) {
        auto dyingEntity = itDying.next();
        dyingEntity.value()--;
        if (dyingEntity.value() < 0) {
            itDying.remove();
        }
    }

    QMutableListIterator<QSharedPointer<Entity>> itEntity(world.entities);
    while (itEntity.hasNext()) {
        auto entity = itEntity.next();
        if (entity->isDying()) {
            world.dyingEntities[entity] = DYING_ANIMATION_TICKS;
            itEntity.remove();
        }
    }

    // 1. 针对每个存活实体，调用updatePosition，计算一个tick内的位移
    //    qDebug() << "1. calculating displacement";
    for (auto entity : world.entities) {
        entity->updatePosition();
    }

    // 2. 计算方块与实体、实体与实体之间的碰撞
    //    qDebug() << "2. calculating collision";
    for (int index = 0; index < world.entities.size(); index++) {
        // 方块与实体
        auto entity = world.entities[index];
        auto entityPosition = entity->getPosition();
        int centerBlockX = static_cast<int>(entityPosition.x()),
            centerBlockY = static_cast<int>(entityPosition.y());
        //        qDebug() << "Check collide #1:" << entity->getName();
        for (int i = centerBlockX - ENTITY_COLLISION_RANGE; i <= centerBlockX + ENTITY_COLLISION_RANGE; i++) {
            for (int j = centerBlockY - ENTITY_COLLISION_RANGE; j <= centerBlockY + ENTITY_COLLISION_RANGE; j++) {
                BlockDelegate blockDelegate(world.getBlock(QPoint(i, j)), QPoint(i, j));
                if (blockDelegate.isAir()) {
                    continue; // 空气方块，跳过检查
                }
                Direction faceOfEntity = entity->checkCollideWith(blockDelegate);
                Direction faceOfBlock = blockDelegate.checkCollideWith(*entity);
                if (faceOfEntity == Direction::UNKNOWN || getOppositeFace(faceOfEntity) != faceOfBlock) {
                    continue; // 未碰撞 / 嵌入其中
                }
                if (faceOfEntity == Direction::DOWN) { // 落地：碰到方块
                    entity->setOnFloor(true);
                }
                // 调用相应的碰撞处理
                entity->collide(blockDelegate, faceOfEntity);
                blockDelegate.collide(*entity, getOppositeFace(faceOfEntity));
                // 处理碰撞后实体的速度
                switch (faceOfEntity) {
                case Direction::UP:
                    entity->setVelocity(QVector2D(entity->getVelocity().x(), -1.0f * entity->getVelocity().y())); // 特殊处理撞到底部
                    entity->setAcceleration(QVector2D(entity->getAcceleration().x(), 0));
                    break;
                case Direction::DOWN:
                    entity->setVelocity(QVector2D(entity->getVelocity().x(), BOUNCE_BOTTOM_ATTENUATION * entity->getVelocity().y()));
                    entity->setAcceleration(QVector2D(entity->getAcceleration().x(), 0));
                    break;
                case Direction::LEFT:
                    entity->setVelocity(QVector2D(0, entity->getVelocity().y()));
                    entity->setAcceleration(QVector2D(0.3f * entity->getVelocity().x(), entity->getAcceleration().y()));
                    break;
                }
                // 处理碰撞后实体的位置，与方块碰撞盒脱离
                float delta = 0.0f;
                switch (faceOfEntity) {
                case Direction::UP:
                    delta = blockDelegate.getBoundingBoxWorld().getMaxY() - entity->getBoundingBoxWorld().getMinY();
                    entity->setPosition(entity->getPosition() + QVector2D(0, delta));
                    break;
                case Direction::DOWN:
                    delta = entity->getBoundingBoxWorld().getMaxY() - blockDelegate.getBoundingBoxWorld().getMinY();
                    entity->setPosition(entity->getPosition() - QVector2D(0, delta));
                    break;
                case Direction::LEFT:
                    delta = blockDelegate.getBoundingBoxWorld().getMaxX() - entity->getBoundingBoxWorld().getMinX();
                    entity->setPosition(entity->getPosition() + QVector2D(delta, 0));
                    break;
                case Direction::RIGHT:
                    delta = entity->getBoundingBoxWorld().getMaxX() - blockDelegate.getBoundingBoxWorld().getMinX();
                    entity->setPosition(entity->getPosition() - QVector2D(delta, 0));
                    break;
                }
            }
        }
        // 实体与实体，注意避免重复计算
        for (int prev = 0; prev < index; prev++) {
            auto anotherEntity = world.entities[prev];

            auto faceOfEntity = entity->checkCollideWith(*anotherEntity);
            auto faceOfAnotherEntity = anotherEntity->checkCollideWith(*entity);

            if (faceOfEntity == parkour::Direction::UNKNOWN || getOppositeFace(faceOfEntity) != faceOfAnotherEntity) {
                continue;
            }
            if (faceOfEntity == Direction::DOWN) { // 落地：碰到实体
                entity->setOnFloor(true);
            }
            entity->collide(*anotherEntity, faceOfEntity);
            anotherEntity->collide(*entity, getOppositeFace(faceOfEntity));

            entity->setPosition(entity->getPosition() - entity->getVelocity() * parkour::TICK_LENGTH);
            anotherEntity->setPosition(anotherEntity->getPosition() - anotherEntity->getVelocity() * parkour::TICK_LENGTH);

            auto newVelocity = (entity->getVelocity() + anotherEntity->getVelocity()) / 2;
            entity->setVelocity(newVelocity);
            anotherEntity->setVelocity(newVelocity);

            auto newAcceleration = (entity->getAcceleration() + anotherEntity->getAcceleration()) / 2; // 默认质量相同
            entity->setAcceleration(newAcceleration);
            anotherEntity->setAcceleration(newAcceleration);


			// 处理碰撞后实体的位置，与另一个脱离
			float delta = 0.0f;
			switch (faceOfEntity) {
			case Direction::UP:
				delta = anotherEntity->getBoundingBoxWorld().getMaxY() - entity->getBoundingBoxWorld().getMinY();
				entity->setPosition(entity->getPosition() + QVector2D(0, delta));
				break;
			case Direction::DOWN:
				delta = entity->getBoundingBoxWorld().getMaxY() - anotherEntity->getBoundingBoxWorld().getMinY();
				entity->setPosition(entity->getPosition() - QVector2D(0, delta));
				break;
			case Direction::LEFT:
				delta = anotherEntity->getBoundingBoxWorld().getMaxX() - entity->getBoundingBoxWorld().getMinX();
				entity->setPosition(entity->getPosition() + QVector2D(delta, 0));
				break;
			case Direction::RIGHT:
				delta = entity->getBoundingBoxWorld().getMaxX() - anotherEntity->getBoundingBoxWorld().getMinX();
				entity->setPosition(entity->getPosition() - QVector2D(delta, 0));
				break;
			}

        }
    }

    // 3. 调用所有实体的更新函数（目前普通方块用于地形，仅仅在受到碰撞时会更新，不参与ticking）
    //    qDebug() << "3. calling update() of entities";
    for (auto entity : world.entities) {
        entity->update();
    }

    // 4. 检查悬空的实体，设置其重力加速度
    //    qDebug() << "4. setting gravity";
    for (auto entity : world.entities) {
        bool isFlying = true;

        for (auto other : world.entities) {
            if (entity->getBoundingBoxWorld().standUpon(other->getBoundingBoxWorld())) {
                isFlying = false;
                break;
            }
        }

        int centerBlockX = static_cast<int>(entity->getPosition().x()),
            centerBlockY = static_cast<int>(entity->getPosition().y());
        for (int i = centerBlockX - ENTITY_COLLISION_RANGE; i <= centerBlockX + ENTITY_COLLISION_RANGE; i++) {
            for (int j = centerBlockY - ENTITY_COLLISION_RANGE; j <= centerBlockY + ENTITY_COLLISION_RANGE; j++) {
                BlockDelegate blockDelegate(world.getBlock(QPoint(i, j)), QPoint(i, j));
                if (blockDelegate.isAir()) {
                    continue; // 空气方块，跳过检查
                }
                if (entity->getBoundingBoxWorld().standUpon(blockDelegate.getBoundingBoxWorld())) {
                    isFlying = false;
                    goto blockChecked; // 用于跳出多层循环
                }
            }
        }
    blockChecked:

        if (isFlying) {
            entity->setOnFloor(false);
            entity->setAcceleration(QVector2D(0, static_cast<float>(GRAVITY)));
        }
    }

    // 5. 更新玩家控制器
    //    qDebug() << "5. updating player controller";
    playerController->tick();

    // 6. 虚空伤害
    for (const auto& entity : world.entities) {
        if (entity->getPosition().y() > WORLD_HEIGHT) {
            entity->setHp(entity->getHp() - VOID_DAMAGE_PER_TICK); // 直接采用setHp / getHp，是因为可能实体子类会覆写 damage() 实现无敌效果，而虚空伤害无视无敌效果
        }
    }

    //    qDebug() << "World Controller ticking done";
}
}
