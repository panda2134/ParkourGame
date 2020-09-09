#ifndef INVENTORYGUI_H
#define INVENTORYGUI_H

#include "guibase.h"
#include "model/item.h"
#include "utils/consts.h"
#include <QEvent>
#include <QImage>
#include <QObject>
#include <QPoint>
#include <QRect>
#include <QSharedPointer>
#include <QSize>
#include <QVector2D>

namespace parkour {
	class InventoryGUI : public GUIBase {
		Q_OBJECT

		int mouseX, mouseY, deviceWidth, deviceHeight, dragIndex;
		QPoint screenTopLeft;
		QVector2D dragOffset;
		QSize screenTextureSize;
		QImage inventoryTexture;
		QRect inventorySlotRects[PLAYER_INVENTORY_SLOT_COUNT];
		QSharedPointer<Item> draggingItem;

		QRect slotOnScreen(const QRect &);
		double getRatio();

	public:
		InventoryGUI();
		~InventoryGUI();

		virtual void paintGUI(QPainter &) override;
		virtual bool event(QEvent * evt) override;

	};
}

#endif
