#ifndef REGISTRY_H
#define REGISTRY_H

#include "../utils/singleton.h"
#include "block.h"
#include "item.h"
#include <map>
#include <string>
#include <QSharedDataPointer>
#include <QMetaObject>
#include <QString>

namespace parkour {

namespace registry {
    typedef std::map<std::string, QSharedPointer<Block>> BlockMapType;

	class ItemRegistry;

    class BlockRegistry : public QObject, public Singleton<BlockRegistry> {
        Q_OBJECT
        BlockMapType blockMap;
        QVector<QString> blockIds;
        std::map<std::string, size_t> idMapping;

		friend class ItemRegistry;

        template <typename T>
        void registerBlock();

    public:
		BlockRegistry();
        QSharedPointer<Block> getBlockByName(QString blockName);
        const QVector<QString>& getBlockIds();
        size_t getBlockIdByName(QString);
    };

	class EntityRegistry : public QObject, public Singleton<EntityRegistry> {
		Q_OBJECT
	public:
		EntityRegistry();
		/*
		 * @brief generateEntity 根据完整(含命名空间)的类名生成实体
		 * @param fullyQualifiedClassName 完整(含命名空间)的类名
		 * @returns 指向实体的{@link QSharedPointer}，如果未找到此类则返回nullptr
		*/
		QSharedPointer<Entity> generateEntity(QString fullyQualifiedClassName);
		QList<QSharedPointer<Entity>> readEntitiesFromStream(QDataStream& in);
		bool writeEntitiesToStream(QDataStream& out, const QList<QSharedPointer<Entity>>& entities);
	};

	class ItemRegistry : public QObject, public Singleton<ItemRegistry> {
		Q_OBJECT
		QList<QSharedPointer<Item> > items;
	public:
		ItemRegistry();
		const QList<QSharedPointer<Item>> & getItems();
	};
}
}

#endif // REGISTRY_H
