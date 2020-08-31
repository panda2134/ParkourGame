#include "world.h"

namespace parkour {

bool World::isReady() const {
    return ready;
}

void World::setReady(bool value) {
    ready = value;
}

void World::setBlock(QPoint blockPos, QString blockName) {
	if (blockPos.x() < 0 || blockPos.x() >= WORLD_WIDTH || 
		blockPos.y() < 0 || blockPos.y() >= WORLD_HEIGHT) {
		return;
	}
	chunks.get(blockPos.x(), blockPos.y()) = blockName;
}

QString World::getBlock(QPoint blockPos) {
    if (blockPos.x() < 0 || blockPos.x() >= WORLD_WIDTH || blockPos.y() < 0 || blockPos.y() >= WORLD_HEIGHT) {
        return "air";
    } else {
        auto& res = chunks.get(blockPos.x(), blockPos.y());
        if (res.size() == 0) {
            res = "air";
        }
        return res;
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
void World::clear() {
	entities.clear();
	dyingEntities.clear();
	for (int j = 0; j < WORLD_HEIGHT; j++) {
		chunks.clear();
	}
}

}
