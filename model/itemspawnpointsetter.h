#ifndef ITEMSPAWNPOINTSETTER_H
#define ITEMSPAWNPOINTSETTER_H

#include "item.h"
#include <QImage>

namespace parkour {
	class ItemSpawnPointSetter : public Item {
		Q_OBJECT

		QImage icon;

	public:
		ItemSpawnPointSetter();
		virtual QString getDisplayName() override;
		virtual const QImage & getIcon() override;
		virtual void onUse(QVector2D) override;

	};

}
#endif // ITEMSPAWNPOINTSETTER_H