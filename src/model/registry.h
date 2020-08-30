#ifndef REGISTRY_H
#define REGISTRY_H

#include "../utils/singleton.h"
#include "block.h"
#include <map>
#include <string>
#include <QSharedDataPointer>
#include <QString>

namespace parkour {

namespace registry {
    typedef std::map<std::string, QSharedPointer<Block>> BlockMapType;

    class BlockRegistry : public QObject, public Singleton<BlockRegistry> {
        Q_OBJECT
        BlockMapType blockMap;
        QVector<QString> blockIds;
        std::map<std::string, size_t> idMapping;

        template <typename T>
        void registerBlock();

    public:
        BlockRegistry();
        QSharedPointer<Block> getBlockByName(QString blockName);
        const QVector<QString>& getBlockIds();
        size_t getBlockIdByName(QString);
    };

}
}

#endif // REGISTRY_H
