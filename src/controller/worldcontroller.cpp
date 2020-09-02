#include "worldcontroller.h"
#include "../model/blockdelegate.h"
#include "../model/entitycreeper.h"
#include "../model/entityplayer.h"
#include "../model/entityblaze.h"
#include "../model/entityslime.h"
#include "../model/registry.h"
#include "../model/world.h"
#include "../utils/consts.h"
#include "../utils/geometryhelper.h"
#include "../vendor/aabbcc/AABB.h"
#include <algorithm>
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

void WorldController::loadTestWorld() const {
    auto& world = World::instance();
    if (world.isReady()) {
        throw std::exception("test world loading failed: world is already ready");
    }
    qDebug() << "setting block";
    for (int i = 0; i <= 2048; i++) {
        world.setBlock(QPoint(i, 15), "bedrock");
    }

    for (int i = 33; i <= 36; i++) {
        world.setBlock(QPoint(i, 14), "spring");
    }

    for (int j = 13; j >= 6; j--) {
        world.setBlock(QPoint(30 - j, j), "oak_plank");
    }
    for (int i = 35; i <= 40; i++) {
        world.setBlock({ i, 14 }, "flower");
    }

	for (int i = 46; i <= 50; i++) {

		for (int j = 0; j <= 14; j++) {
			world.setBlock({ i, j }, "dirt");
		}

		for (int j = 14; j >= 10; j--) {
			world.setBlock({ i, j }, "tnt");
		}
	}


    qDebug() << "loading player";

    world.setSpawnPoint({ 29, 10 });

    QSharedPointer<Entity> e = QSharedPointer<EntitySlime>::create();
    e->placeBoundingBoxAt({ 12.0f, 14.0f });
    world.addEntity(e);

	e = QSharedPointer<EntitySlime>::create();
	e->placeBoundingBoxAt({ 10.0f, 14.0f });
	world.addEntity(e);

	e = QSharedPointer<EntitySlime>::create();
	e->placeBoundingBoxAt({ 8.0f, 14.0f });
	world.addEntity(e);

	e = QSharedPointer<EntitySlime>::create();
	e->placeBoundingBoxAt({ 6.0f, 14.0f });
	world.addEntity(e);

	e = QSharedPointer<EntityCreeper>::create();
	e->placeBoundingBoxAt({ 2.0f, 14.0f });
	world.addEntity(e);

	e = QSharedPointer<EntityBlaze>::create();
	e->placeBoundingBoxAt({ 0.0f, 14.0f });
	world.addEntity(e);

    qDebug() << "test world loaded";
    world.setReady(true);
}

void WorldController::unloadWorld() const {
    auto& world = World::instance();
    if (!world.isReady()) {
        throw std::exception("world not ready, cannot unload");
    }
	world.clear();
    world.setReady(false);
}

void WorldController::saveWorld() {
	QDataStream out(&save, QIODevice::WriteOnly);
	serializeWorld(out);
}

void WorldController::loadWorld() {
	QDataStream in(&save, QIODevice::ReadOnly);
	deserializeWorld(in);
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
        throw std::exception("world not ready");
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
			auto velocity = (power * EXPLOSION_VELOCITY_MULTIPLIER) * QVector2D(entityPointF - center).normalized() * (radius - distance);
            entity->damage(damage);
            if (entity->isAffectedByExplosionWave()) {
                entity->setVelocity(entity->getVelocity() + velocity);
            }
        }
    }
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
		//long long p = 0, q = 0;
		aabb::Tree aabbTree(2, 0.05, world.entities.size());
		for (int index = 0; index < world.entities.size(); index++) {
			// 方块与实体
			auto entity = world.entities[index];
			auto entityPosition = entity->getPosition();
			auto bbox = entity->getBoundingBoxWorld();
			aabbTree.insertParticle(index, { bbox.getMinX(), bbox.getMinY() }, { bbox.getMaxX(), bbox.getMaxY() });
			int centerBlockX = static_cast<int>(entityPosition.x()),
				centerBlockY = static_cast<int>(entityPosition.y());
			int range = std::max({ static_cast<float>(ENTITY_COLLISION_RANGE), bbox.getMaxY() - bbox.getMinY() + 1.0f, bbox.getMaxX() - bbox.getMinX() + 1.0f });

			for (int i = centerBlockX - range; i <= centerBlockX + range; i++) {
				for (int j = centerBlockY - range; j <= centerBlockY + range; j++) {
					const auto& blockName = world.getBlock(QPoint(i, j));
					/*QElapsedTimer t31;
					t31.start();*/
					BlockDelegate blockDelegate(blockName, QPoint(i, j));
					/*p += t31.nsecsElapsed();*/

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

			//QElapsedTimer t32;
			//t32.start();

			// 实体与实体，注意避免重复计算
			
			for (auto collideId : aabbTree.query(index)) {
				if (collideId == index) {
					continue; // 跳过自己
				}
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

				auto v1 = (m1 - m2) / (m1 + m2) * entity->getVelocity(),
					v2 = (2 * m1) / (m1 + m2) * anotherEntity->getVelocity();

				switch (faceOfEntity) {
				case Direction::UP: case Direction::DOWN:
					entity->setVelocity({ entity->getVelocity().x(), v1.y() });
					anotherEntity->setVelocity({ anotherEntity->getVelocity().x(), v2.y() });
					break;
				case Direction::LEFT: case Direction::RIGHT:
					entity->setVelocity({ v1.x(), entity->getVelocity().y() });
					anotherEntity->setVelocity({ v2.x(), anotherEntity->getVelocity().y() });
					break;
				}

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
                        //q += t32.nsecsElapsed();
                }
                //qDebug() << "step3: " << p / 1e6 << q / 1e6;
        }

        // 4. 调用所有实体的更新函数（目前普通方块用于地形，仅仅在受到碰撞时会更新，不参与ticking）
        //    qDebug() << "4. calling update() of entities";
        //        auto entities2 = world.entities;
        for (const auto& entity : world.entities) {
            entity->update();
        }

        // 5. 检查悬空的实体，设置其重力加速度
        //    qDebug() << "5. setting gravity";
        {
            aabb::Tree aabbTree(2);
            for (int i = 0; i < world.entities.size(); i++) {
                auto& entity = world.entities[i];
                const auto& bbox = entity->getBoundingBoxWorld();

                aabbTree.insertParticle(i, { bbox.getMinX(), bbox.getMinY() }, { bbox.getMaxX(), bbox.getMaxY() });

                bool isFlying = true;

                for (auto j : aabbTree.query(i)) {
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
