#include "registry.h"
#include "blockdirt.h"
#include "blockgrass.h"
#include "blocktnt.h"
#include "blocksaw.h"
#include "blockbedrock.h"

namespace parkour {
namespace registry {

    template <typename T>
    void BlockRegistry::registerBlock() {
        auto ptr = QSharedPointer<T>::create();
        blockMap.insert(ptr->getName(), ptr);
        blockIds.push_back(ptr->getName());
        idMapping[ptr->getName()] = blockIds.size() - 1;
    }

    BlockRegistry::BlockRegistry() {

        blockIds.push_back("air"); // 空出id=0给air
        idMapping["air"] = 0;
        // * block mapping start *
        registerBlock<parkour::BlockGrass>();
        registerBlock<parkour::BlockDirt>();
        registerBlock<parkour::BlockTNT>();
		registerBlock<parkour::BlockSaw>();
		registerBlock<parkour::BlockBedrock>();
        // * block mapping end   *
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
