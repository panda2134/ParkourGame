#include "entityslime.h"
#include "entityplayer.h"
#include "entityxporb.h"
#include "view/scene/gamesound.h"
#include "world.h"
#include <QRandomGenerator>

namespace parkour {
EntitySlime::EntitySlime()
    : waitTicksLeft(-1) {
}
QString EntitySlime::getName() const {
    return "slime";
}
QString EntitySlime::getResourceLocation() {
    return ":/assets/entities/slime.png";
}
QVector2D EntitySlime::getTextureDimensions() {
    return QVector2D(1.0f, 1.0f);
}
BoundingBox EntitySlime::getBoundingBox() const {
    return BoundingBox { { 0.125f, 0.125f }, { 0.75f, 0.75f } };
}
double EntitySlime::getWalkSpeed() const {
    return 1.5;
}
double EntitySlime::getMass() const {
    return 6000;
}

void EntitySlime::collide(ICollidable& other, Direction dir) {
    if (dir == Direction::DOWN) {
        this->waitTicksLeft = WAIT_TIMEOUT_MIN + (WAIT_TIMEOUT_MAX - WAIT_TIMEOUT_MIN) * QRandomGenerator::global()->generateDouble();
        this->setVelocity({ .0f, -0.1f * getVelocity().y() });
        if (other.getType() == "entity") {
            this->setVelocity({ 5 * float(QRandomGenerator::global()->generateDouble() * 2 - 1), float(-JUMP_SPEED * QRandomGenerator::global()->generateDouble()) });
        }
        GameSound::instance().playWorldSound(
            QString("Slime_big%1").arg(QRandomGenerator::system()->generate() % 4 + 1),
            getPosition());
    }
    if (other.getName() == "player") {
        auto& player = static_cast<EntityPlayer&>(other);
        player.damage(DAMAGE_PER_HIT + (player.getVelocity() - getVelocity()).length() * COLLISION_MULTIPLIER);
    }
}
void EntitySlime::update() {
    if (waitTicksLeft >= 0) {
        waitTicksLeft--;
    } else {
        if (!isOnFloor()) {
            return;
        }
        auto& world = World::instance();
        for (auto& entity : world.getEntities()) {
            if (entity->getName() == "player") {
                if (entity->getPosition().x() < getPosition().x()) {
                    jump();
                    goLeft();
                } else {
                    jump();
                    goRight();
                }
                break;
            }
        }
    }
}
QString EntitySlime::getDisplayName() const {
    return "史莱姆";
}
void EntitySlime::damage(double value) {
    EntityPlayerLike::damage(value);
    if (this->getHp() < 0) {
        GameSound::instance().playWorldSound(
            QString("Slime_big%1").arg(QRandomGenerator::system()->generate() % 4 + 1),
            getPosition());
        EntityXpOrb::dropXpOrbs(getPosition(), 2);
    } else {
        GameSound::instance().playWorldSound(
            QString("Slime_small%1").arg(QRandomGenerator::system()->generate() % 4 + 1),
            getPosition());
    }
}
double EntitySlime::getMaxHp() const {
    return 6.0;
}
void EntitySlime::jump() {
    EntityPlayerLike::jump();
    GameSound::instance().playWorldSound(QString("Slime_small%1").arg(QRandomGenerator::system()->generate() % 4 + 1), getPosition());
}
void EntitySlime::serializeCustomProps(QDataStream& out) const {
    out << waitTicksLeft;
}
void EntitySlime::deserializeCustomProps(QDataStream& in) {
    in >> waitTicksLeft;
}
int EntitySlime::getSerializationVersion() const {
    return 1;
}
}