#include "registry.h"

namespace parkour {
namespace registry {

    BlockRegistry::BlockRegistry() {
#define REGISTER_BLOCK(Cls)                              \
    {                                                    \
        auto ptr = QSharedPointer<Cls>::create();        \
        blockMap.insert(ptr->getName(), ptr);            \
        blockIds.push_back(ptr->getName());              \
        idMapping[ptr->getName()] = blockIds.size() - 1; \
    }

        blockIds.push_back("air"); // 空出id=0给air
        idMapping["air"] = 0;
        // * block mapping start *
        REGISTER_BLOCK(parkour::BlockGrass);
        REGISTER_BLOCK(parkour::BlockDirt);
        // * block mapping end   *

#undef REGISTER_BLOCK
    }

    QSharedPointer<Block> BlockRegistry::getBlockByName(QString blockName) {
        if (blockName == "air") {
            return nullptr;
        } else if (blockMap.count(blockName) == 0) {
            throw std::exception((blockName + ": no such block!").toStdString().c_str());
        } else {
            return blockMap[blockName];
        }
    }

    size_t BlockRegistry::getBlockIdByName(QString blockName) {
        return idMapping[blockName];
    }

    const QVector<QString>& BlockRegistry::getBlockIds() {
        return blockIds;
    }

}
}
