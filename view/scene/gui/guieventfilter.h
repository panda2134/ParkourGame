#ifndef GUIEVENTFILTER_H
#define GUIEVENTFILTER_H

#include <QObject>

namespace parkour {
	class GUIEventFilter : public QObject {
		Q_OBJECT
		Qt::Key exitKey;

	public:
		GUIEventFilter(Qt::Key key = Qt::Key_Escape, QObject *parent = nullptr);
		~GUIEventFilter();
		virtual bool eventFilter(QObject *watched, QEvent *evt) override;
	signals:
		void quitGUI();
	};
}

#endif