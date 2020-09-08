#ifndef ITEMMOVINGBRICK_H
#define ITEMMOVINGBRICK_H

#include "item.h"
#include <QImage>
#include <QVector2D>

namespace parkour {
	class ItemMovingBrick : public Item {
		Q_OBJECT

		QImage icon;
		bool firstPointSelected = false;
		QVector2D firstPoint;
	public:
		ItemMovingBrick();
		virtual QString getDisplayName() override;
		virtual const QImage & getIcon() override;
		virtual void onUse(QVector2D) override;
	};
}

#endif