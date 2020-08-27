#ifndef REGISTRY_H
#define REGISTRY_H

#include "../utils/singleton.h"
#include "block.h"
#include "blockdirt.h"
#include "blockgrass.h"
#include <QHash>
#include <QSharedDataPointer>
#include <QString>

namespace parkour {

namespace registry {
    typedef QHash<QString, QSharedPointer<Block>> BlockMapType;

    class BlockRegistry : public QObject, public Singleton<BlockRegistry> {
        Q_OBJECT
        BlockMapType blockMap;
        QVector<QString> blockIds;
        QHash<QString, size_t> idMapping;

    public:
        BlockRegistry();
        QSharedPointer<Block> getBlockByName(QString blockName);
        const QVector<QString>& getBlockIds();
        size_t getBlockIdByName(QString);
    };

}
}

#endif // REGISTRY_H
