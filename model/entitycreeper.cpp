#include "entitycreeper.h"
#include "controller/worldcontroller.h"
#include "entityplayer.h"
#include "entityxporb.h"
#include "view/scene/gamesound.h"
#include "world.h"
#include <QRandomGenerator>
#include <QSharedPointer>

namespace parkour {
EntityCreeper::EntityCreeper()
    : randomTicksLeft(-1) {
}

QString EntityCreeper::getName() const {
    return "creeper";
}

QString EntityCreeper::getResourceLocation() {
    return ":/assets/entities/creeper.png";
}

QVector2D EntityCreeper::getTextureDimensions() {
    return QVector2D(1, 2);
}

BoundingBox EntityCreeper::getBoundingBox() const {
    return BoundingBox { { 0.25f, 0.5f }, { 0.5f, 1.5f } };
}

void EntityCreeper::update() {
    auto& world = World::instance();
    bool found = false;
    QVector2D target;
    for (auto& entity : world.getEntities()) {
        if (entity->getName() == "player") {
            target = entity->getPosition();
            found = true;
            break;
        }
    }
    auto delta = target - getPosition();
    if (!found || delta.lengthSquared() > AI_SEARCH_RANGE * AI_SEARCH_RANGE) {
        return;
    }

    if (delta.lengthSquared() < EXPLOSION_THRESHOLD) {
        if (countdown < 0) {
            countdown = FUSE_LENGTH;
            GameSound::instance().playWorldSound("Creeper_fuse", getPosition());
        } else {
            countdown--;
        }
    } else {
        countdown = -1;
        if (randomTicksLeft > 0) {
            randomTicksLeft--;
        } else {
            if (delta.x() > 1) {
                goRight();
            } else if (delta.x() < -1) {
                goLeft();
            } else {
                auto randomGen = QRandomGenerator::global();
                randomTicksLeft = randomGen->generateDouble() * RANDOM_TICK_LENGTH;
                if (randomGen->generateDouble() > 0.5) {
                    goRight();
                } else {
                    goLeft();
                }
            }
        }
    }

    if (countdown == 0) {
        this->setHp(-1);
        WorldController::instance().explode(getPosition().toPoint(), TNT_EXPLOSION_POWER);
    }
}
void EntityCreeper::collide(ICollidable& other, Direction dir) {
    if (dir == Direction::LEFT || dir == Direction::RIGHT) {
        jump();
    }
}
double EntityCreeper::getWalkSpeed() const {
    return 1.5;
}
bool EntityCreeper::showDeathAnimationAndInfo() const {
    return true;
}
QString EntityCreeper::getDisplayName() const {
    return "爬行者";
}
void EntityCreeper::damage(double val) {
    EntityPlayerLike::damage(val);
    if (this->getHp() < 0) {
        EntityXpOrb::dropXpOrbs(getPosition(), 5);
    }
}
void EntityCreeper::serializeCustomProps(QDataStream& out) const {
    out << randomTicksLeft;
}
void EntityCreeper::deserializeCustomProps(QDataStream& in) {
    in >> randomTicksLeft;
}
int EntityCreeper::getSerializationVersion() const {
    return 1;
}
}