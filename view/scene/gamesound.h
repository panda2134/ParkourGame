#ifndef GAMESOUND_H
#define GAMESOUND_H

#include "utils/singleton.h"
#include <QObject>
#include <QHash>
#include <QString>
#include <QThread>
#include <QVector2D>
#include <QSharedPointer>
#include <QSoundEffect>

namespace parkour {
	class GameSound : public QObject, public Singleton<GameSound> {
		Q_OBJECT

		QHash<QString, QSharedPointer<QSoundEffect> > sounds;
		void loadSound(const QString&);

		class GameSoundWorker: public QObject {
			GameSound* parent;
		public:
			GameSoundWorker(GameSound* parent) {
				this->parent = parent;
			}
			void playSound(QString name);
			void playWorldSound(QString name, QVector2D position);
		};

		GameSoundWorker* worker = nullptr;
		QThread th;
		
	signals:
		void playSoundInWorker(QString);
		void playWorldSoundInWorker(QString, QVector2D);

	public:
		GameSound();
		~GameSound();
		void playSound(const QString&);
		void playWorldSound(const QString&, const QVector2D &);
	};
}

#endif
