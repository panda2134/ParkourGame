#pragma once

#include "item.h"
#include "entity.h"
#include <QString>
#include <QSharedPointer>

namespace parkour {
	class ItemSpawnEgg : public Item {
		Q_OBJECT

		QString entityClassName;
		QImage icon;

	public:
		ItemSpawnEgg(QString entityClassName_, QString iconPath_);
		virtual QString getDisplayName() override;
		virtual const QImage & getIcon() override;
		virtual void onUse(QVector2D) override;
	};
}