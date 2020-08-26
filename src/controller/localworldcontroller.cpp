#include "localworldcontroller.h"
#include "../model/testentity.h"

namespace parkour {
LocalWorldController::LocalWorldController(QObject* parent)
    : QObject(parent) {
}

void LocalWorldController::loadTestWorld() {
    auto& world = World::instance();
    if (world.isReady()) {
        throw std::exception("test world loading failed: world is already ready");
    }
    for (int i = 1; i <= 100; i++) {
        world.setBlock(qMakePair(i, 16), "grass");
    }
    auto e1 = QSharedPointer<TestEntity>(new TestEntity(QVector2D(0, 0), QVector2D(1, 1)));
    e1->setPosition(QVector2D(1, 15));
    e1->setVelocity(QVector2D(1, 0));
}

void LocalWorldController::tick() {
    auto& world = World::instance();
    ++world.ticksFromBirth;

    // 0. 维护将死亡实体列表，并把entities中将死亡的实体加入列表
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
    for (auto entity : world.entities) {
        entity->updatePosition();
    }

    // 2. 计算方块与实体、实体与实体之间的碰撞
    for (int index = 0; index < world.entities.size(); index++) {
        // 方块与实体
        auto entity = world.entities[index];
        auto entityPosition = entity->getPosition();
        int centerBlockX = static_cast<int>(entityPosition.x()),
            centerBlockY = static_cast<int>(entityPosition.y());
        for (int i = centerBlockX - ENTITY_UPDATE_RANGE; i <= centerBlockX + ENTITY_UPDATE_RANGE; i++) {
            for (int j = centerBlockY - ENTITY_UPDATE_RANGE; j <= centerBlockY + ENTITY_UPDATE_RANGE; j++) {
                BlockDelegate blockDelegate(world.getBlock(qMakePair(i, j)), qMakePair(i, j));
                if (blockDelegate.isAir()) {
                    continue; // 空气方块，跳过检查
                }
                Direction faceOfEntity = entity->checkCollideWith(blockDelegate);
                if (faceOfEntity == Direction::UNKNOWN) {
                    continue; // 未碰撞 / 嵌入其中
                }
                // 调用相应的碰撞处理
                entity->collide(blockDelegate, faceOfEntity);
                blockDelegate.collide(*entity, getOppositeFace(faceOfEntity));
                // 处理碰撞后实体的速度
                switch (faceOfEntity) {
                case Direction::UP:
                case Direction::DOWN:
                    entity->setVelocity(QVector2D(entity->getVelocity().x(), 0));
                    break;
                case Direction::LEFT:
                case Direction::RIGHT:
                    entity->setVelocity(QVector2D(0, entity->getVelocity().y()));
                    break;
                default:
                    throw std::exception("This is impossible");
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
            if (faceOfEntity == parkour::Direction::UNKNOWN) {
                continue;
            }
            using parkour::geometry::normalizeDenominater;
            entity->collide(*anotherEntity, faceOfEntity);
            anotherEntity->collide(*entity, getOppositeFace(faceOfEntity));

            entity->setPosition(entity->getPosition() - entity->getVelocity() * parkour::TICK_LENGTH);
            anotherEntity->setPosition(anotherEntity->getPosition() - anotherEntity->getVelocity() * parkour::TICK_LENGTH);

            auto newVelocity = (entity->getVelocity() + anotherEntity->getVelocity()) / 2;
            entity->setVelocity(newVelocity);
            anotherEntity->setVelocity(newVelocity);
            entity->setPosition(entity->getPosition() + parkour::TICK_LENGTH * newVelocity); // 补一帧，修复共同加速之情况
            anotherEntity->setPosition(anotherEntity->getPosition() + parkour::TICK_LENGTH * newVelocity);
        }
    }

    // 3. 调用所有实体的更新函数（目前普通方块用于地形，仅仅在受到碰撞时会更新，不参与ticking）
    for (auto entity : world.entities) {
        entity->update();
    }
}
}
