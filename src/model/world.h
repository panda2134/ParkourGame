#ifndef WORLD_H
#define WORLD_H

#include "../utils/singleton.h"
#include "entity.h"
#include <QDebug>
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
    void setBlock(QPoint blockPos, QString blockName);
    QString getBlock(QPoint blockPos);

    void addEntity(QSharedPointer<Entity>);

    const QList<QSharedPointer<Entity>>& getEntities() const;

    size_t getTicksFromBirth() const;

    const QHash<QSharedPointer<Entity>, int>& getDyingEntities() const;

private:
    bool ready = false;
    size_t ticksFromBirth = 0;
    QList<QSharedPointer<Entity>> entities;
    QHash<QSharedPointer<Entity>, int> dyingEntities;
    QHash<QPair<int, int>, QString> blocks;
};
}

#endif // WORLD_H
