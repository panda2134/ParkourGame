#ifndef REGISTRY_H
#define REGISTRY_H

#include "block.h"
#include "blockgrass.h"
#include <QHash>
#include <QSharedDataPointer>
#include <QString>

namespace parkour {

namespace registry {
    typedef QHash<QString, QSharedPointer<Block>> BlockMapType;

    /**
     * @brief _initializeBlockMap 初始化方块映射，采用宏简化映射过程
     * @param blockMap 方块映射
     * @private
     */
    void _initializeBlockMap(BlockMapType& blockMap) {
#define _INSERT_INTO_BLOCK_MAP(Cls)                \
    {                                              \
        auto ptr = QSharedPointer<Cls>(new Cls()); \
        blockMap.insert(ptr->getName(), ptr);      \
    }
        // * block mapping start *
        _INSERT_INTO_BLOCK_MAP(parkour::BlockGrass);
        // * block mapping end   *
#undef _INSERT_INTO_BLOCK_MAP
    }

    QSharedPointer<Block> getBlockByName(QString blockName) {
        static BlockMapType blockMap;
        if (blockMap.size() == 0) {
            _initializeBlockMap(blockMap);
        }
        if (blockName == "air") {
            return nullptr;
        } else if (blockMap.count(blockName) == 0) {
            throw std::exception((blockName + ": no such block!").toStdString().c_str());
        } else {
            return blockMap[blockName];
        }
    }

}

}

#endif // REGISTRY_H
