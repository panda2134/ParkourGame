#include "entityblaze.h"
#include "../utils/consts.h"
#include "entityfireball.h"
#include "world.h"
#include <QRandomGenerator>
#include <QSharedPointer>

namespace parkour {
void EntityBlaze::pushAttackQueue(QSharedPointer<Entity> entity) {
    auto& world = World::instance();
    const auto &target = entity->getPosition(), current = getPosition();
    auto vec = QVector2D(target - current);
    auto norm = vec.normalized();
    const auto start = current + norm;
    const float DIVISION_COUNT = 10;

    bool ok = true;

    for (auto offset = QVector2D(0, 0);
         (start + offset - target).lengthSquared() > 1;
         offset += norm / DIVISION_COUNT) {
        const auto& blockPos = (start + offset).toPoint();
        if (world.getBlock(blockPos) != "air") {
            ok = false;
            break;
        }
    }

    if (ok) {
        for (int i = 1; i <= FIREBALLS_PER_ATTACK; i++) {
            attackQueue.push_back(qMakePair(livingTicks + SHOOT_INTERVAL * i, norm));
        }
    }
}

EntityBlaze::EntityBlaze()
    : livingTicks(0)
    , cooldown(-1)
    , state(BlazeState::UP) {
}

void EntityBlaze::collide(ICollidable& other, Direction dir) {
    Q_UNUSED(other)

    if (dir == Direction::UP) {
        state = BlazeState::HIGHEST;
    } else if (dir == Direction::DOWN) {
        state = BlazeState::LOWEST;
    }
}

QString EntityBlaze::getName() const {
    return "blaze";
}

void EntityBlaze::update() {
    auto& world = World::instance();
    livingTicks++;

    if (state == BlazeState::UP && (getPosition().y() < 1 || world.getBlock(QPoint(getPosition().x(), getPosition().y() - 1.0f)) != "air" || world.getBlock(QPoint(getPosition().x() - 1.0f, getPosition().y() - 1.0f)) != "air" || world.getBlock(QPoint(getPosition().x() + 1.0f, getPosition().y() - 1.0f)) != "air")) {
        state = BlazeState::HIGHEST;
    } else if (state == BlazeState::DOWN && (getPosition().y() >= WORLD_HEIGHT || world.getBlock(QPoint(getPosition().x(), getPosition().y() + 1.0f)) != "air" || world.getBlock(QPoint(getPosition().x() - 1.0f, getPosition().y() + 1.0f)) != "air" || world.getBlock(QPoint(getPosition().x() + 1.0f, getPosition().y() + 1.0f)) != "air")) {
        state = BlazeState::LOWEST;
    } else {
        // 在停住时，随机决定是否改变状态
        if (QRandomGenerator::global()->generateDouble() < CHANGE_STATE_POSSIBILITY) {
            state = (state == BlazeState::HIGHEST) ? BlazeState::DOWN : BlazeState::UP;
        }
    }

    // 攻击玩家
    //    推入攻击队列
    if (QRandomGenerator::global()->generateDouble() < ATTACK_POSSIBILITY && cooldown < livingTicks) {
        cooldown = livingTicks + COOLDOWN_TICKS;
        for (auto& entity : world.getEntities()) {
            if (entity->getName() == "player") {
                pushAttackQueue(entity);
            }
        }
    }
    //    处理攻击队列
    for (auto& [tick, norm] : attackQueue) {
        if (tick <= livingTicks) {
			auto bbox = getBoundingBoxWorld();
			EntityFireball::placeFireball(bbox.getCenter() + norm * 2, norm * FIREBALL_VELOCITY);
        }
    }
    while (!attackQueue.empty() && attackQueue.front().first <= livingTicks) {
        attackQueue.pop_front();
    }

    // 根据状态设置速度
    switch (state) {
    case BlazeState::UP:
        setVelocity({ getVelocity().x(), -1.0f * VERTICAL_VELOCITY });
        break;
    case BlazeState::DOWN:
        setVelocity({ getVelocity().x(), 1.0f * VERTICAL_VELOCITY });
        break;
    case BlazeState::HIGHEST:
    case BlazeState::LOWEST:
        setVelocity({ getVelocity().x(), 0.0f });
        break;
    }
}

bool EntityBlaze::isAffectedByGravity() const {
    return false;
}

QString EntityBlaze::getResourceLocation() {
    const auto frameNum = (livingTicks / BLAZE_FRAME_LENGTH) & 1;
    return QString(":/assets/entities/blaze/%1.png").arg(frameNum);
}

QVector2D EntityBlaze::getTextureDimensions() {
    return { 1, 1 };
}

BoundingBox EntityBlaze::getBoundingBox() const {
    return BoundingBox { { 0.25f, 0.0625f }, { 0.5f, 0.5f } };
}
double EntityBlaze::getMass() const {
	return 1e10;
}
void EntityBlaze::serializeCustomProps(QDataStream & out) const {
	out << livingTicks << cooldown << state << attackQueue;
}
void EntityBlaze::deserializeCustomProps(QDataStream & in) {
	in >> livingTicks >> cooldown >> state >> attackQueue;
}
int EntityBlaze::getSerializationVersion() const {
	return 1;
}
}
