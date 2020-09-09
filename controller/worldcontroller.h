#ifndef WORLDCONTROLLER_H
#define WORLDCONTROLLER_H

#include "../utils/singleton.h"
#include "../utils/consts.h"
#include "playercontroller.h"
#include <QObject>
#include <QString>
#include <QSharedPointer>
#include <QTimer>
#include <QElapsedTimer>

namespace parkour {
class WorldController : public QObject, public Singleton<WorldController> {
	Q_OBJECT

    const int EXPLOSIONS_PER_TICK = 2;
    const bool NO_EXPLOSION = false;
    const double AMBIENT_PLAY_CHANCE = 1.0 / (300 * TICKS_PER_SEC);

    QSharedPointer<PlayerController> playerController;
    void handleExplosion(QPoint center, double power) const;

public:
    WorldController();

    void loadInitialWorld() const;
    void unloadWorld() const;

    void serializeWorld(QDataStream& out);
    void deserializeWorld(QDataStream& in);

    void explode(QPoint center, double power) const;
    QSharedPointer<PlayerController> getPlayerController() const;

public slots:
    void tick() const;
};
}

#endif // WORLDCONTROLLER_H
