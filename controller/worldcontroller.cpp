#include "worldcontroller.h"
#include "utils/worldioworker.h"
#include "utils/consts.h"
#include "utils/geometryhelper.h"
#include "model/blockdelegate.h"
#include "model/entitycreeper.h"
#include "model/entityplayer.h"
#include "model/entityblaze.h"
#include "model/entityslime.h"
#include "model/registry.h"
#include "model/world.h"
#include "view/scene/gamesound.h"
#include <algorithm>
#include <QDebug>
#include <QRandomGenerator>
#include <QByteArray>
#include <QDataStream>

namespace parkour {

QSharedPointer<PlayerController> WorldController::getPlayerController() const
{
    return playerController;
}

WorldController::WorldController()
    : playerController(QSharedPointer<PlayerController>::create()) {
}

void WorldController::loadInitialWorld() const {
	WorldIOWorker::instance().setProgress(0);
    auto& world = World::instance();
    if (world.isReady()) {
        throw "test world loading failed: world is already ready";
    }
    for (int i = 0; i < WORLD_WIDTH; i++) {
        world.setBlock(QPoint(i, 15), "bedrock");
		int progress = 100.0 / WORLD_WIDTH * i;
		if (progress != WorldIOWorker::instance().getProgress()) {
			WorldIOWorker::instance().setProgress(progress);
		}
    }

    world.setSpawnPoint({ 1, 12 });
    world.setReady(true);
}

void WorldController::unloadWorld() const {
    auto& world = World::instance();
    if (!world.isReady()) {
        throw "world not ready, cannot unload";
    }
	world.clear();
    world.setReady(false);
}

void WorldController::serializeWorld(QDataStream & out) {
	out.setVersion(QDATASTREAM_VERSION);
	out << MAP_SERIALIZATION_VERSION;
	out << World::instance();
}

void WorldController::deserializeWorld(QDataStream & in) {
	in.setVersion(QDATASTREAM_VERSION);
	QString version; in >> version;

	if (version != MAP_SERIALIZATION_VERSION) {
		in.setStatus(QDataStream::ReadCorruptData); 
		return;
	}

	in >> World::instance();
}

void WorldController::explode(QPoint center, double power) const {
	auto& world = World::instance();
	world.explosionQueue.push_back(World::ExplosionInfo(center, power));
}

void WorldController::handleExplosion(QPoint center, double power) const {
    if (NO_EXPLOSION || power < 0) {
        return;
    }
    auto& world = World::instance();
    if (!world.isReady()) {
        throw "world not ready";
    }
    // damage blocks
    double radius = power * EXPLOSION_RADIUS_MULTIPLIER;
    for (int i = center.x() - radius; i <= center.x() + radius; i++) {
        for (int j = center.y() - radius; j <= center.y() + radius; j++) {
            auto distance = QVector2D(center - QPoint(i, j)).length();
            auto block = registry::BlockRegistry::instance().getBlockByName(world.getBlock({ i, j }));
            if (block != nullptr && geometry::compareDoubles(radius - distance, block->getExplosionResistance()) >= 0) {
                if (QPoint(i, j) != center) {
                    block->onExplosion({ i, j }, radius - distance);
                }
                world.setBlock({ i, j }, "air");
            }
        }
    }
    for (auto& entity : world.getEntities()) {
		const auto& entityPointF = entity->getPosition().toPointF();
        auto distance = QVector2D(entityPointF - center).length();
        if (geometry::compareDoubles(distance, radius) <= 0) {
            auto damage = (power * EXPLOSION_DAMAGE_MULTIPLIER / (radius * radius)) * (distance - radius) * (distance - radius);
			auto velocity = (power * EXPLOSION_VELOCITY_MULTIPLIER) * QVector2D(entityPointF - center).normalized() * (radius - distance) / entity->getMass();
            entity->damage(damage);
            if (entity->isAffectedByExplosionWave()) {
                entity->setVelocity(entity->getVelocity() + velocity);
            }
        }
    }
    GameSound::instance().playSound(
        QString("Explode%1").arg(QRandomGenerator::system()->generate() % 4 + 1)
    );
}

void WorldController::tick() const {
    auto& world = World::instance();

    if (!world.isReady()) {
        throw "ticking in a non-ready world";
    }

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

    // 2. 处理爆炸队列
    for (int i = 0; i < EXPLOSIONS_PER_TICK && !world.explosionQueue.empty(); i++) {
        auto info = world.explosionQueue.front();
        world.explosionQueue.pop_front();
        this->handleExplosion(info.center, info.power);
    }

    // 3. 计算方块与实体、实体与实体之间的碰撞
	if (world.entities.size() > 0) {
		for (int index = 0; index < world.entities.size(); index++) {
			// 方块与实体
			auto entity = world.entities[index];
			auto entityPosition = entity->getPosition();
			auto bbox = entity->getBoundingBoxWorld();
			int centerBlockX = static_cast<int>(entityPosition.x()),
				centerBlockY = static_cast<int>(entityPosition.y());
			int range = std::max({ static_cast<float>(ENTITY_COLLISION_RANGE), bbox.getMaxY() - bbox.getMinY() + 1.0f, bbox.getMaxX() - bbox.getMinX() + 1.0f });

			for (int i = centerBlockX - range; i <= centerBlockX + range; i++) {
				for (int j = centerBlockY - range; j <= centerBlockY + range; j++) {
					const auto& blockName = world.getBlock(QPoint(i, j));
					BlockDelegate blockDelegate(blockName, QPoint(i, j));

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
						entity->setVelocity(QVector2D(entity->getVelocity().x(), -1.0f * BOUNCE_BOTTOM_ATTENUATION * entity->getVelocity().y()));
						entity->setAcceleration(QVector2D(entity->getAcceleration().x(), 0));
						break;
					case Direction::LEFT:
					case Direction::RIGHT:
						entity->setVelocity(QVector2D(-1.0f * BOUNCE_SIDE_ATTENUATION * entity->getVelocity().x(), entity->getVelocity().y()));
						entity->setAcceleration(QVector2D(0, entity->getAcceleration().y()));
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
			
			for (int collideId = 0; collideId < index; ++collideId) {
				auto anotherEntity = world.entities[collideId];

				auto faceOfEntity = entity->checkCollideWith(*anotherEntity);
				auto faceOfAnotherEntity = anotherEntity->checkCollideWith(*entity);

				if (faceOfEntity == Direction::UNKNOWN || getOppositeFace(faceOfEntity) != faceOfAnotherEntity) {
					continue;
				}
				if (faceOfEntity == Direction::DOWN) { // 落地：碰到实体
					entity->setOnFloor(true);
				}
				entity->collide(*anotherEntity, faceOfEntity);
				anotherEntity->collide(*entity, getOppositeFace(faceOfEntity));

				auto m1 = entity->getMass(), m2 = anotherEntity->getMass();
				auto v1 = entity->getVelocity(), v2 = anotherEntity->getVelocity();

				auto p = m1 * entity->getVelocity() + m2 * anotherEntity->getVelocity();

				auto v1n = (m1 - m2) / (m1 + m2) * (v1 - v2) + v2,
					v2n = (2 * m1) / (m1 + m2) * (v1 - v2) + v2;
				auto vn = p / (m1 + m2);

				switch (faceOfEntity) {
				case Direction::UP: case Direction::DOWN:
					entity->setVelocity({ vn.x(), v1n.y() });
					anotherEntity->setVelocity({ vn.x(), v2n.y() });
					break;
				case Direction::LEFT: case Direction::RIGHT:
					entity->setVelocity({ v1n.x(), vn.y() });
					anotherEntity->setVelocity({ v2n.x(), vn.y() });
					break;
				}

				auto lighter = entity, heavier = anotherEntity;
				if (lighter->getMass() > heavier->getMass()) {
					qSwap(lighter, heavier);
				}

				float delta = 0.0f;
				switch (lighter->checkCollideWith(*heavier)) {
				case Direction::UP:
					delta = qMax(.0f, heavier->getBoundingBoxWorld().getMaxY() - lighter->getBoundingBoxWorld().getMinY());
					lighter->setPosition(lighter->getPosition() + QVector2D(0, delta));
					break;
				case Direction::DOWN:
					delta = qMax(.0f, lighter->getBoundingBoxWorld().getMaxY() - heavier->getBoundingBoxWorld().getMinY());
					lighter->setPosition(lighter->getPosition() - QVector2D(0, delta));
					break;
				case Direction::LEFT:
					delta = qMax(.0f, heavier->getBoundingBoxWorld().getMaxX() - lighter->getBoundingBoxWorld().getMinX());
					lighter->setPosition(lighter->getPosition() + QVector2D(delta, 0));
					break;
				case Direction::RIGHT:
                    delta = qMax(.0f, lighter->getBoundingBoxWorld().getMaxX() - heavier->getBoundingBoxWorld().getMinX());
					lighter->setPosition(lighter->getPosition() - QVector2D(delta, 0));
                    break;
                }
            }
        }
    }

        // 4. 调用所有实体的更新函数（目前普通方块用于地形，仅仅在受到碰撞时会更新，不参与ticking）
        for (const auto& entity : world.entities) {
            entity->update();
        }

        // 5. 检查悬空的实体，设置其重力加速度
        {
            for (int i = 0; i < world.entities.size(); i++) {
                auto& entity = world.entities[i];
                const auto& bbox = entity->getBoundingBoxWorld();

                bool isFlying = true;

				for (int j = 0; j < world.entities.size(); j++) {
                    if (i == j) {
                        continue;
                    }
                    auto& other = world.entities[j];
                    if (entity->getBoundingBoxWorld().standUpon(other->getBoundingBoxWorld())) {
                        isFlying = false;
                        break;
                    }
                }

                int centerBlockX = static_cast<int>(entity->getPosition().x()),
                    centerBlockY = static_cast<int>(entity->getPosition().y());
                int range = std::max({ static_cast<float>(ENTITY_COLLISION_RANGE), bbox.getMaxY() - bbox.getMinY() + 1.0f, bbox.getMaxX() - bbox.getMinX() + 1.0f });
                for (int i = centerBlockX - range; i <= centerBlockX + range; i++) {
                    for (int j = centerBlockY - range; j <= centerBlockY + range; j++) {
                        const auto& blockName = world.getBlock(QPoint(i, j));
                        BlockDelegate blockDelegate(blockName, QPoint(i, j));
                        if (blockDelegate.isAir()) {
                            continue; // 空气方块，跳过检查
                        }
                        if (entity->getBoundingBoxWorld().standUpon(blockDelegate.getBoundingBoxWorld())) {
                            isFlying = false;
                            auto block = registry::BlockRegistry::instance().getBlockByName(blockName);
                            // block != null, since it is not air
                            block->onStand({ i, j }, *entity);
                        }
                    }
                }

                if (isFlying) {
                    entity->setOnFloor(false);
                    if (entity->isAffectedByGravity()) {
                        entity->setAcceleration(QVector2D(0, static_cast<float>(GRAVITY)));
                    }
                }
            }
        }

    // 6. 更新玩家控制器
    playerController->tick();

    // 7. 虚空伤害
    for (const auto& entity : world.entities) {
        if (entity->getPosition().y() > WORLD_HEIGHT) {
            entity->setHp(entity->getHp() - VOID_DAMAGE_PER_TICK); // 直接采用setHp / getHp，是因为可能实体子类会覆写 damage() 实现无敌效果，而虚空伤害无视无敌效果
        }
    }
}
}
