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
#include "blockoakplank.h"

#include "entity.h"
#include "entityblaze.h"
#include "entitycreeper.h"
#include "entityfireball.h"
#include "entitymovingbrick.h"
#include "entityplayer.h"
#include "entityplayerlike.h"
#include "entityslime.h"
#include "entitytnt.h"
#include "entityxporb.h"

#include "itemblock.h"
#include "itemspawnegg.h"

#include <QDebug>


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
		registerBlock<parkour::BlockOakPlank>();
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
		// * entity registry end   *

#undef REGISTER_ENTITY
	}

	QSharedPointer<Entity> EntityRegistry::generateEntity(QString fullyQualifiedClassName) {
		fullyQualifiedClassName += "*"; // 转为指针类型
		int type = QMetaType::type(fullyQualifiedClassName.toLocal8Bit().constData());
		if (type == QMetaType::UnknownType) {
			return nullptr;
		}
		const QMetaObject *mo = QMetaType::metaObjectForType(type);
		return QSharedPointer<Entity>(static_cast<Entity*>(mo->newInstance()));
	}

	QList<QSharedPointer<Entity>> EntityRegistry::readEntitiesFromStream(QDataStream & in) {
		QList<QSharedPointer<Entity>> ret;
		int count;
		if ((in >> count).status() != QDataStream::Ok) {
			qDebug() << "read: count corrupted, cannot read";
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
			if (in.status() != QDataStream::Ok) {
				continue;
			}
			ret.push_back(entity);
		}
		return ret;
	}

	bool EntityRegistry::writeEntitiesToStream(QDataStream & out, 
		const QList<QSharedPointer<Entity>>& entities) {

		int count = entities.size();
		out << count;
		qDebug() << "write count = " << count;
		for (const auto &entity : entities) {
			QString className = entity->metaObject()->className();
			qDebug() << "write entity of class " << className;
			out << className;
			out << *entity;
		}

		return true;
	}

	ItemRegistry::ItemRegistry() {
		// 为每个方块注册对应的物品
		const auto &blockMap = BlockRegistry::instance().blockMap;
		for (const auto &[_, block] : blockMap) {
			items.push_back(QSharedPointer<ItemBlock>::create(block));
		}
		// 注册刷怪蛋和移动方块生成器
		items.push_back(QSharedPointer<ItemSpawnEgg>::create(
			"parkour::EntityCreeper",
			":/assets/items/spawn_egg_creeper.png"
		));
		items.push_back(QSharedPointer<ItemSpawnEgg>::create(
			"parkour::EntityBlaze",
			":/assets/items/spawn_egg_blaze.png"
		));
		items.push_back(QSharedPointer<ItemSpawnEgg>::create(
			"parkour::EntitySlime",
			":/assets/items/spawn_egg_slime.png"
		));
		items.push_back(QSharedPointer<ItemSpawnEgg>::create(
			"parkour::EntityMovingBrick",
			":/assets/blocks/stone_brick.png"
		));

		// debug
		for (const auto &item : items) {
			qDebug() << item->getDisplayName();
		}
	}

	const QList<QSharedPointer<Item>>& ItemRegistry::getItems() {
		return items;
	}

}
}
