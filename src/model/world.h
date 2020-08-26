#ifndef WORLD_H
#define WORLD_H

#include "../utils/singleton.h"
#include "../utils/types.h"
#include "entity.h"
#include <QHash>
#include <QList>
#include <QObject>
#include <QPair>
#include <QString>

namespace parkour {
class LocalWorldController;
class World : public QObject, public Singleton<World> {
    Q_OBJECT

    friend class LocalWorldController;

public:
    bool isReady() const;
    void setReady(bool value);
    void setBlock(BlockPosType blockPos, QString blockName);
    QString getBlock(BlockPosType blockPos);

private:
    bool ready = false;
    size_t ticksFromBirth = 0;
    QList<QSharedPointer<Entity>> entities;
    QHash<QSharedPointer<Entity>, int> dyingEntities;
    QHash<BlockPosType, QString> blocks;
};
}

#endif // WORLD_H
