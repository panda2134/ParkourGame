#include "world.h"
#include "registry.h"
#include "../controller/worldioworker.h"

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
		if (blockPos.y() < 0) {
			return "bedrock";
		} else {
			return "air";
		}
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

void World::removeEntity(QSharedPointer<Entity> entity) {
	for (auto it = entities.begin(); it != entities.end(); ++it) {
		if (*it == entity) {
			entities.erase(it);
			break;
		}
	}
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

QDataStream & operator<<(QDataStream & out, const World & world) {
	WorldIOWorker::instance().setProgress(0);
	// world metadata
	out << WORLD_SERIALIZATION_VERSION
		<< world.ticksFromBirth << world.spawnPoint;
	WorldIOWorker::instance().setProgress(5);

	// blocks
	for (int chunkNum = 0; chunkNum < World::CHUNK_COUNT; chunkNum++) {
		bool created = world.chunks.isChunkCreated(chunkNum);
		out << created;
		if (created) {
			for (int y = 0; y < WORLD_HEIGHT; y++) {
				for (int x = 0; x < CHUNK_SIZE; x++) {
					out << world.chunks.data[chunkNum][y * CHUNK_SIZE + x];
				}
			}
		}
		WorldIOWorker::instance().setProgress(WorldIOWorker::instance().getProgress() + 50.0 / World::CHUNK_COUNT);
	}

	// entities
	registry::EntityRegistry::instance().writeEntitiesToStream(out, world.entities);

	WorldIOWorker::instance().setProgress(99);

	return out;
}

QDataStream & operator>>(QDataStream & in, World & world) {
	QString version;
	in >> version;
	if (version != WORLD_SERIALIZATION_VERSION) {
		in.setStatus(QDataStream::ReadCorruptData);
		return in;
	}
	in >> world.ticksFromBirth >> world.spawnPoint; // world metadata

	// blocks
	for (int chunkNum = 0; chunkNum < World::CHUNK_COUNT; chunkNum++) {
		bool created;
		in >> created;
		if (created) {
			for (int y = 0; y < WORLD_HEIGHT; y++) {
				for (int x = 0; x < CHUNK_SIZE; x++) {
					in >> world.chunks.get(x + chunkNum * CHUNK_SIZE, y);
				}
			}
		}
	}

	// entities
	world.entities = registry::EntityRegistry::instance().readEntitiesFromStream(in);

	WorldIOWorker::instance().setProgress(99);

	return in;
}

}
