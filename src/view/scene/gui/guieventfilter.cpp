#include "guieventfilter.h"
#include <QEvent>
#include <QKeyEvent>

namespace parkour {
	GUIEventFilter::GUIEventFilter(GameScene &parent, Qt::Key key) : exitKey(key), parent(parent) {
	}

	GUIEventFilter::~GUIEventFilter() {}

	bool GUIEventFilter::eventFilter(QObject *watched, QEvent *evt) {
		if (evt->type() == QEvent::KeyPress) {
			auto *ke = static_cast<QKeyEvent*>(evt);
			if (ke->key() == Qt::Key::Key_Escape || ke->key() == exitKey) {
				qDebug("esc pressed in gui");
				parent.closeGUI();
				return true;
			}
		}
		return false;
	}
}