#include "registry.h"

#include "block.h"
#include "blockbedrock.h"
#include "blockdirt.h"
#include "blockexpbottle.h"
#include "blockflower.h"
#include "blockgrass.h"
#include "blockmushroom.h"
#include "blockoakplank.h"
#include "blocksaw.h"
#include "blockspike.h"
#include "blockspring.h"
#include "blockstonebrick.h"
#include "blocktnt.h"

#include "entity.h"
#include "entityblaze.h"
#include "entitycreeper.h"
#include "entityfireball.h"
#include "entitymovingbrick.h"
#include "entityobserver.h"
#include "entityplayer.h"
#include "entityplayerlike.h"
#include "entityslime.h"
#include "entitytnt.h"
#include "entityxporb.h"

#include "itemblock.h"
#include "itemmovingbrick.h"
#include "itemspawnegg.h"
#include "itemspawnpointsetter.h"
#include "itemteleporter.h"

#include "../utils/worldioworker.h"

#include <QDebug>

namespace parkour {
namespace registry {

    template <typename T>
    void BlockRegistry::registerBlock() {
        QSharedPointer<Block> ptr = QSharedPointer<T>::create();
        blockMap[ptr->getName().toStdString()] = ptr;
        blockIds.push_back(ptr->getName());
        idMapping[ptr->getName().toStdString()] = static_cast<size_t>(blockIds.size()) - 1;
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
        registerBlock<parkour::BlockOakPlank>();
        registerBlock<parkour::BlockExpBottle>();
        // * block mapping end   *
    }

    QSharedPointer<Block> BlockRegistry::getBlockByName(QString blockName) {
        if (blockName == "air") {
            return nullptr;
        } else {
            const auto& blockNameStd = blockName.toStdString();
            if (blockMap.count(blockNameStd) == 0) {
                qDebug() << (blockName + ": no such block!").toStdString().c_str();
                return nullptr;
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

    EntityRegistry::EntityRegistry() {
#define REGISTER_ENTITY(T) qRegisterMetaType<T*>("parkour::" #T "*")

        // * entity registry start *
        REGISTER_ENTITY(EntityBlaze);
        REGISTER_ENTITY(EntityCreeper);
        REGISTER_ENTITY(EntityFireball);
        REGISTER_ENTITY(EntityMovingBrick);
        REGISTER_ENTITY(EntityPlayer);
        REGISTER_ENTITY(EntitySlime);
        REGISTER_ENTITY(EntityTNT);
        REGISTER_ENTITY(EntityXpOrb);
        REGISTER_ENTITY(EntityObserver);
        // * entity registry end   *

#undef REGISTER_ENTITY
    }

    QSharedPointer<Entity> EntityRegistry::generateEntity(QString fullyQualifiedClassName) {
        fullyQualifiedClassName += "*"; // 转为指针类型
        int type = QMetaType::type(fullyQualifiedClassName.toLocal8Bit().constData());
        if (type == QMetaType::UnknownType) {
            return nullptr;
        }
        const QMetaObject* mo = QMetaType::metaObjectForType(type);
        return QSharedPointer<Entity>(static_cast<Entity*>(mo->newInstance()));
    }

    QList<QSharedPointer<Entity>> EntityRegistry::readEntitiesFromStream(QDataStream& in) {
        QList<QSharedPointer<Entity>> ret;
        auto& ioWorker = WorldIOWorker::instance();
        int count;
        if ((in >> count).status() != QDataStream::Ok) {
            qDebug() << "read: count corrupted, cannot read";
            ioWorker.setProgress(0);
            return ret;
        }
        for (int i = 0; i < count; i++) {
            QString className;
            in >> className;
            auto entity = generateEntity(className);
            if (entity == nullptr) {
                qDebug() << "skipped unknown entity class" << className;
                continue;
            }
            in >> *entity;
            ioWorker.setProgress(ioWorker.getProgress() + 50.0 / count);

            if (in.status() != QDataStream::Ok) {
                continue;
            }
            ret.push_back(entity);
        }
        return ret;
    }

    bool EntityRegistry::writeEntitiesToStream(QDataStream& out,
        const QList<QSharedPointer<Entity>>& entities) {
        auto& ioWorker = WorldIOWorker::instance();
        int count = entities.size();
        out << count;
        for (const auto& entity : entities) {
            QString className = entity->metaObject()->className();
            out << className;
            out << *entity;
            ioWorker.setProgress(ioWorker.getProgress() + 50.0 / count);
        }

        return true;
    }

    ItemRegistry::ItemRegistry() {
        // 为每个方块注册对应的物品
        const auto& blockMap = BlockRegistry::instance().blockMap;
        for (const auto& [_, block] : blockMap) {
            items.push_back(QSharedPointer<ItemBlock>::create(block));
        }
        // 注册刷怪蛋
        items.push_back(QSharedPointer<ItemSpawnEgg>::create(
            "parkour::EntityCreeper",
            ":/assets/items/spawn_egg_creeper.png"));
        items.push_back(QSharedPointer<ItemSpawnEgg>::create(
            "parkour::EntityBlaze",
            ":/assets/items/spawn_egg_blaze.png"));
        items.push_back(QSharedPointer<ItemSpawnEgg>::create(
            "parkour::EntitySlime",
            ":/assets/items/spawn_egg_slime.png"));
        items.push_back(QSharedPointer<ItemSpawnEgg>::create(
            "parkour::EntityObserver",
            ":/assets/items/observer.png"));
        items.push_back(QSharedPointer<ItemMovingBrick>::create());
        items.push_back(QSharedPointer<ItemSpawnPointSetter>::create());
        items.push_back(QSharedPointer<ItemTeleporter>::create());
    }

    const QList<QSharedPointer<Item>>& ItemRegistry::getItems() {
        return items;
    }

}
}
