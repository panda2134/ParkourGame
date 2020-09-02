#ifndef WORLD_H
#define WORLD_H

#include "../utils/singleton.h"
#include "../utils/consts.h"
#include "entity.h"
#include <map>
#include <QDebug>
#include <QHash>
#include <QList>
#include <QObject>
#include <QPair>
#include <QString>
#include <QQueue>
#include <QDataStream>

namespace parkour {
class WorldController;
const QString WORLD_SERIALIZATION_VERSION = "WORLD1";
class World : public QObject, public Singleton<World> {
    Q_OBJECT

	static const size_t CHUNK_COUNT = static_cast<size_t>((1.0 * WORLD_WIDTH) / CHUNK_SIZE + 0.5);

	struct ExplosionInfo {
		QPoint center;
		double power;
		ExplosionInfo(QPoint center_, double power_): center(center_), power(power_) {}
	};

	template<typename T>
	struct ChunkStorage {
		T *data[CHUNK_COUNT];

	public:
		ChunkStorage() {
			for (int i = 0; i < CHUNK_COUNT; i++) {
				data[i] = nullptr;
			}
		}
		T& get(int x, int y) {
            Q_ASSERT(x >= 0 && x < WORLD_WIDTH && y >= 0 && y < WORLD_HEIGHT);
            auto chunkNum = x / CHUNK_SIZE;
            if (data[chunkNum] == nullptr) {
                data[chunkNum] = new T[CHUNK_SIZE * WORLD_HEIGHT];
            }
            return data[chunkNum][y * CHUNK_SIZE + (x % CHUNK_SIZE)];
        }
		bool isChunkCreated(int chunkNum) const {
			return data[chunkNum] != nullptr;
		}
		void clear() {
			for (int i = 0; i < CHUNK_COUNT; i++) {
				delete[] data[i];
				data[i] = nullptr;
			}
		}
		~ChunkStorage() {
			clear();
		}
	};
	bool ready = false;
	size_t ticksFromBirth = 0;
	QVector2D spawnPoint;
	QList<QSharedPointer<Entity>> entities;
	QHash<QSharedPointer<Entity>, int> dyingEntities;
	ChunkStorage<QString> chunks;
	QQueue<ExplosionInfo> explosionQueue;
	void clear();

    friend class WorldController;

public:
    bool isReady() const;
    void setReady(bool value);
    void setBlock(QPoint blockPos, QString blockName);
    QString getBlock(QPoint blockPos);
    void addEntity(QSharedPointer<Entity>);
    const QList<QSharedPointer<Entity>>& getEntities() const;
	/**
	 * @brief removeEntity 从实体列表移除entity实体
	 * @note 调用后，所有的实体列表迭代器立刻失效！必须立刻结束迭代或退出range-based for loop
	 */
	void removeEntity(QSharedPointer<Entity> entity);
    size_t getTicksFromBirth() const;
    const QHash<QSharedPointer<Entity>, int>& getDyingEntities() const;
    QVector2D getSpawnPoint() const;
    void setSpawnPoint(const QVector2D& value);

	friend QDataStream& operator<<(QDataStream& out, const World &world);
	friend QDataStream& operator>>(QDataStream& in, World &world);

};
}

#endif // WORLD_H
