#include "gamescene.h"
#include "gamesound.h"
#include "controller/playercontroller.h"
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

		// load required sound files
		for (int i = 1; i <= 3; i++) {
			loadSound(QString("Glass_dig%1").arg(i));
		}

		loadSound("Exp_gained");
		loadSound("Levelup");
		loadSound("Fireball");

		loadSound("Blaze_death");
		for (int i = 1; i <= 4; i++) {
			loadSound(QString("Blaze_hurt%1").arg(i));
			loadSound(QString("Blaze_idle%1").arg(i));
		}
		for (int i = 1; i <= 4; i++) {
			loadSound(QString("Slime_big%1").arg(i));
			loadSound(QString("Slime_small%1").arg(i));
		}
		loadSound("Fuse");
		loadSound("Creeper_fuse");
		for (int i = 1; i <= 4; i++) {
			loadSound(QString("Explode%1").arg(i));
		}
	}

	GameSound::~GameSound() {
		th.quit();
		th.wait();
		if (worker != nullptr) {
			delete worker;
		}
	}

	void parkour::GameSound::playSound(const QString& name) {
		emit playSoundInWorker(name);
	}

	void parkour::GameSound::GameSoundWorker::playSound(QString name) {
		if (this->parent->sounds[name] != nullptr) {
			this->parent->sounds[name]->stop();
			this->parent->sounds[name]->play();
		}
	}
}