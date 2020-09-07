#include "guieventfilter.h"
#include <QEvent>
#include <QKeyEvent>
#include <QDebug>

namespace parkour {
	GUIEventFilter::GUIEventFilter(Qt::Key key, QObject *parent) : exitKey(key), QObject(parent) {
	}

	GUIEventFilter::~GUIEventFilter() {}

	bool GUIEventFilter::eventFilter(QObject *watched, QEvent *evt) {
		if (evt->type() == QEvent::KeyPress) {
			auto *ke = static_cast<QKeyEvent*>(evt);
			if (ke->key() == Qt::Key::Key_Escape || ke->key() == exitKey) {
				emit quitGUI();
				return true;
			}
		}
		return false;
	}
}