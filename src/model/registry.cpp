#include "registry.h"
#include "block.h"
#include "blockbedrock.h"
#include "blockdirt.h"
#include "blockgrass.h"
#include "blocksaw.h"
#include "blockspike.h"
#include "blockspring.h"
#include "blockstonebrick.h"
#include "blocktnt.h"
#include "blockmushroom.h"
#include "blockflower.h"

namespace parkour {
namespace registry {

    template <typename T>
    void BlockRegistry::registerBlock() {
        QSharedPointer<Block> ptr = QSharedPointer<T>::create();
        blockMap[ptr->getName().toStdString()] = ptr;
        blockIds.push_back(ptr->getName());
        idMapping[ptr->getName().toStdString()] = blockIds.size() - 1;
    }

    BlockRegistry::BlockRegistry() {

        blockIds.push_back("air"); // 空出id=0给air
        idMapping["air"] = 0;
        // * block mapping start *
        registerBlock<parkour::BlockGrass>();
        registerBlock<parkour::BlockDirt>();
        registerBlock<parkour::BlockTNT>();
        registerBlock<parkour::BlockSaw>();
        registerBlock<parkour::BlockSpike>();
        registerBlock<parkour::BlockBedrock>();
        registerBlock<parkour::BlockStoneBrick>();
		registerBlock<parkour::BlockSpring>();
		registerBlock<parkour::BlockMushroom>();
		registerBlock<parkour::BlockFlower>();
        // * block mapping end   *
    }

    QSharedPointer<Block> BlockRegistry::getBlockByName(QString blockName) {
        if (blockName == "air") {
            return nullptr;
		} else {
			const auto &blockNameStd = blockName.toStdString();
			if (blockMap.count(blockNameStd) == 0) {
				throw std::exception((blockName + ": no such block!").toStdString().c_str());
			} else {
				return blockMap[blockNameStd];
			}
		}
    }

    size_t BlockRegistry::getBlockIdByName(QString blockName) {
        return idMapping[blockName.toStdString()];
    }

    const QVector<QString>& BlockRegistry::getBlockIds() {
        return blockIds;
    }

}
}
