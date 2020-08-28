#include "world.h"

namespace parkour {

bool World::isReady() const {
    return ready;
}

void World::setReady(bool value) {
    ready = value;
}

void World::setBlock(QPoint blockPos, QString blockName) {
    auto pair = qMakePair(blockPos.x(), blockPos.y());
    blocks[pair] = blockName;
}

QString World::getBlock(QPoint blockPos) {
    auto pair = qMakePair(blockPos.x(), blockPos.y());
    if (blocks.count(pair)) {
        return blocks[pair];
    } else {
        return "air";
    }
}

void World::addEntity(QSharedPointer<Entity> entity) {
    for (auto x : entities) {
        if (x == entity) {
            return; // already added
        }
    }
    entities.push_back(entity);
}

const QList<QSharedPointer<Entity>>& World::getEntities() const {
    return entities;
}

size_t World::getTicksFromBirth() const {
    return ticksFromBirth;
}

const QHash<QSharedPointer<Entity>, int>& World::getDyingEntities() const {
    return dyingEntities;
}

QVector2D World::getSpawnPoint() const {
    return spawnPoint;
}

void World::setSpawnPoint(const QVector2D& value) {
    spawnPoint = value;
}
}
