#ifndef ITEMTELEPORTER_H
#define ITEMTELEPORTER_H

#include "item.h"
#include <QObject>
#include <QImage>

namespace parkour {
	class ItemTeleporter : public Item {
		Q_OBJECT

		QImage icon;

	public:
		ItemTeleporter();
		virtual QString getDisplayName() override;
		virtual const QImage & getIcon() override;
		virtual void onUse(QVector2D) override;
	};
}

#endif