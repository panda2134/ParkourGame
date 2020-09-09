#include "gamesound.h"
#include <QDebug>

namespace parkour {
	void parkour::GameSound::loadSound(const QString& name) {
		sounds[name] = QSharedPointer<QSoundEffect>::create();
		sounds[name]->setSource(QUrl("qrc:/assets/sounds/" + name + ".wav"));
		sounds[name]->setLoopCount(1);
	}

	parkour::GameSound::GameSound() {
		worker = new GameSoundWorker(this);
		worker->moveToThread(&th);
		connect(this, &GameSound::playSoundInWorker, worker, &GameSoundWorker::playSound);
		th.start();

		for (int i = 1; i <= 3; i++) {
			loadSound(QString("Glass_dig%1").arg(i));
		}
		loadSound("Exp_gained");
		loadSound("Levelup");
	}

	GameSound::~GameSound() {
		th.quit();
		th.wait();
	}

	void parkour::GameSound::playSound(const QString& name) {
		qDebug() << "send play req to thread" << name;
		emit playSoundInWorker(name);
	}

	void parkour::GameSound::GameSoundWorker::playSound(QString name) {
		if (this->parent->sounds[name] != nullptr) {
			qDebug() << "play sound" << name;
			this->parent->sounds[name]->play();
		}
	}
}