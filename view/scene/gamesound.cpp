#include "gamescene.h"
#include "gamesound.h"
#include "controller/worldcontroller.h"
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
		connect(this, &GameSound::playWorldSoundInWorker, worker, &GameSoundWorker::playWorldSound);
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

	void GameSound::playSound(const QString& name) {
		emit playSoundInWorker(name);
	}

	void GameSound::playWorldSound(const QString& name, const QVector2D& position) {
		emit playWorldSoundInWorker(name, position);
	}

	void GameSound::GameSoundWorker::playSound(QString name) {
		auto& sound = this->parent->sounds[name];
		if (sound != nullptr) {
			sound->stop();
			sound->play();
		}
	}
	void GameSound::GameSoundWorker::playWorldSound(QString name, QVector2D position) {
		const double MAX_RANGE = 32;

		auto player = parkour::WorldController::instance().getPlayerController()->getPlayer();
		if (player != nullptr) {
			double distance = (player->getPosition() - position).length();
			double volume = distance <= MAX_RANGE ? qMin(1.0, 10.0 / (distance * distance)) : 0;
			auto& sound = this->parent->sounds[name];
			if (sound != nullptr) {
				sound->stop();
				sound->setVolume(volume);
				qDebug() << "play world sound" << name << position << volume;
				sound->play();
			}
		}
	}
}