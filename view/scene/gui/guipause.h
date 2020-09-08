#ifndef GUIPAUSE_H
#define GUIPAUSE_H
#include "guibase.h"
#include <QImage>
#include <QEvent>
#include <QObject>

namespace parkour {
	class GUIPause : public GUIBase {
		Q_OBJECT

		static const int BUTTON_CNT = 2;
		int mouseX, mouseY;
		QImage widgetTexture;
		QRect buttonRects[BUTTON_CNT];
	public:
		GUIPause();
		virtual void paintGUI(QPainter &) override;
		virtual bool event(QEvent *evt) override;

	signals:
		void saveAndExit();
	};
}

#endif