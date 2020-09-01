#ifndef WORLDCONTROLLER_H
#define WORLDCONTROLLER_H

#include "../utils/singleton.h"
#include "../vendor/aabbcc/AABB.h"
#include "playercontroller.h"
#include <QObject>
#include <QString>
#include <QSharedPointer>
#include <QTimer>
#include <QElapsedTimer>

namespace parkour {
	const int QDATASTREAM_VERSION = QDataStream::Qt_5_14;
	const QString MAP_SERIALIZATION_VERSION = "MAPFILE1";
class WorldController : public QObject, public Singleton<WorldController> {
	Q_OBJECT

    const int EXPLOSIONS_PER_TICK = 2;
    const bool NO_EXPLOSION = false;
	
	QByteArray save;

    QSharedPointer<PlayerController> playerController;
    void handleExplosion(QPoint center, double power) const;

public:
    WorldController();

    void loadTestWorld() const;
    void unloadWorld() const;

	void executeSerializationTests();

	void saveWorld();
	void loadWorld();

	void serializeWorld(QDataStream& out);
	void deserializeWorld(QDataStream& in);

    void explode(QPoint center, double power) const;
    QSharedPointer<PlayerController> getPlayerController() const;

public slots:
    void tick() const;
signals:
};
}

#endif // WORLDCONTROLLER_H
