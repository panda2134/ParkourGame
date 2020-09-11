#include "entityblaze.h"
#include "entityfireball.h"
#include "entityxporb.h"
#include "utils/consts.h"
#include "view/scene/gamesound.h"
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
    auto gen = QRandomGenerator::global();
    livingTicks++;

    if (state == BlazeState::UP && (getPosition().y() < 1 || blocked(-1.0f))) {
        state = BlazeState::HIGHEST;
    } else if (state == BlazeState::DOWN && (getPosition().y() >= WORLD_HEIGHT || blocked(1.0f) || blocked(2.0f) || blocked(3.0f))) {
        state = BlazeState::LOWEST;
    } else {
        // 在停住时，随机决定是否改变状态
        if (QRandomGenerator::global()->generateDouble() < CHANGE_STATE_POSSIBILITY) {
            state = (state == BlazeState::HIGHEST) ? BlazeState::DOWN : BlazeState::UP;
            GameSound::instance().playWorldSound(QString("Blaze_idle%1").arg(gen->generate() % 4 + 1), getPosition());
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
void EntityBlaze::damage(double val) {
    Entity::damage(val);
    auto gen = QRandomGenerator::global();
    GameSound::instance().playWorldSound(QString("Blaze_hurt%1").arg(gen->generate() % 4 + 1), getPosition());
    if (this->getHp() < 0) {
        GameSound::instance().playWorldSound("Blaze_death", getPosition());
        int count = gen->generate() % 3 + 1;
        for (int i = 0; i < count; i++) {
            EntityXpOrb::dropXpOrbs(getPosition(), gen->generateDouble() * 10);
        }
    }
}
QString EntityBlaze::getDisplayName() const {
    return "烈焰人";
}
void EntityBlaze::serializeCustomProps(QDataStream& out) const {
    out << livingTicks << cooldown << state << attackQueue;
}
void EntityBlaze::deserializeCustomProps(QDataStream& in) {
    in >> livingTicks >> cooldown >> state >> attackQueue;
}
int EntityBlaze::getSerializationVersion() const {
    return 1;
}
bool EntityBlaze::blocked(float offsetY) {
    auto& world = World::instance();
    return world.getBlock(QPoint(getPosition().x(), getPosition().y() + offsetY)) != "air"
        || world.getBlock(QPoint(getPosition().x() - 1.0f, getPosition().y() + offsetY)) != "air"
        || world.getBlock(QPoint(getPosition().x() + 1.0f, getPosition().y() + offsetY)) != "air";
}
}
