#ifndef GUIBASE_H
#define GUIBASE_H
#include <QObject>
#include <QString>
#include <QPainter>

namespace parkour {
	class GUIBase : public QObject {
		Q_OBJECT

	protected:
		

	public:
		/**
		 * @brief paintGUI 画出GUI页面，使用参数给定的QPainter
		 */
		virtual void paintGUI(QPainter &) = 0;

	signals:
		void quitGUI();
	};
}

#endif