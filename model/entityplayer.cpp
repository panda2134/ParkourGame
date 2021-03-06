﻿#include "entityplayer.h"
#include "entityxporb.h"
#include "registry.h"
#include "utils/experiencehelper.h"
#include "view/scene/gamesound.h"

namespace parkour {

EntityPlayer::EntityPlayer()
    : maxHp(20.0)
    , EntityPlayerLike() {
    this->exp = 0;
    this->shootFireballs = false;
    this->loadInventory();
}

BoundingBox EntityPlayer::getBoundingBox() const {
    if (isSneak()) {
        return BoundingBox { QVector2D(0.25, 1.00), QVector2D(0.5, 0.98) };
    } else {
        return BoundingBox { QVector2D(0.25, 0.15), QVector2D(0.5, 1.85) };
    }
}

QString EntityPlayer::getName() const {
    return "player";
}

QString EntityPlayer::getResourceLocation() {
    if (!isSneak()) {
        return ":/assets/entities/player/0.png";
    } else {
        return ":/assets/entities/player/1.png";
    }
}

QVector2D EntityPlayer::getTextureDimensions() {
    return QVector2D(1, 2);
}

void EntityPlayer::update() {
    if (this->isOnFloor()) {
        const float vx = this->getVelocity().x();
        if (qAbs(vx) < GROUND_STOP_THRESHOLD) {
            this->setVelocity({ 0.0f, this->getVelocity().y() });
            this->setAcceleration({ 0.0f, this->getAcceleration().y() });
        } else {
            this->setAcceleration({ static_cast<float>(-1 * DRAG_FACTOR * vx * vx * vx / qAbs(vx)), this->getAcceleration().y() });
        }
    }
}

void EntityPlayer::setMaxHp(double value) {
    maxHp = value;
}

double EntityPlayer::getMaxHp() const {
    return maxHp;
}

int EntityPlayer::getExp() const {
    return exp;
}

void EntityPlayer::increaseExp(int delta) {
    auto origLevel = ExperienceHelper(exp).toLevelInfo();
    exp += delta;
    GameSound::instance().playWorldSound("Exp_gained", getPosition());
    if (std::get<0>(ExperienceHelper(exp).toLevelInfo()) > std::get<0>(origLevel)) {
        GameSound::instance().playWorldSound("Levelup", getPosition());
    }
}

bool EntityPlayer::isAbleToShootFireballs() {
    return shootFireballs;
}

QString EntityPlayer::getDisplayName() const {
    return "玩家";
}

void EntityPlayer::setShootFireballs(bool value) {
    this->shootFireballs = value;
}

void EntityPlayer::damage(double value) {
    EntityPlayerLike::damage(value);
    if (getHp() < 0) {
        EntityXpOrb::dropXpOrbs(getPosition(), getExp() / 2);
    }
}

QSharedPointer<Item>* EntityPlayer::getInventory() {
    return inventory;
}

void EntityPlayer::serializeCustomProps(QDataStream& out) const {
    out << maxHp << shootFireballs << this->exp;
}

void EntityPlayer::deserializeCustomProps(QDataStream& in) {
    in >> maxHp >> shootFireballs >> this->exp;
}

int EntityPlayer::getSerializationVersion() const {
    return 1;
}

double EntityPlayer::getWalkSpeed() const {
    return isSneak() ? WALK_SPEED * 0.3 : WALK_SPEED;
}

void EntityPlayer::loadInventory() {
    const auto& items = registry::ItemRegistry::instance().getItems();
    Q_ASSERT(items.length() <= PLAYER_INVENTORY_SLOT_COUNT);
    for (int i = 0; i < items.length(); i++) {
        if (i < 9) {
            inventory[3 * 9 + i] = items[i];
        } else {
            inventory[i - 9] = items[i];
        }
    }
}
}