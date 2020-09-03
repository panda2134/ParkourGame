#ifndef GUIEVENTFILTER_H
#define GUIEVENTFILTER_H

#include <QObject>
#include "../gamescene.h"

namespace parkour {
	class GUIEventFilter : public QObject {
		Q_OBJECT
		GameScene &parent;
		Qt::Key exitKey;

	public:
		GUIEventFilter(GameScene &parent, Qt::Key key = Qt::Key_Escape);
		~GUIEventFilter();
		virtual bool eventFilter(QObject *watched, QEvent *evt) override;
	};
}

#endif