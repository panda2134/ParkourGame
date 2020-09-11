#ifndef WORLDCONTROLLER_H
#define WORLDCONTROLLER_H

#include "../utils/consts.h"
#include "../utils/singleton.h"
#include "playercontroller.h"
#include <QElapsedTimer>
#include <QObject>
#include <QSharedPointer>
#include <QString>
#include <QTimer>

namespace parkour {
class WorldController : public QObject, public Singleton<WorldController> {
    Q_OBJECT

    const int EXPLOSIONS_PER_TICK = 2;
    const bool NO_EXPLOSION = false;
    const double AMBIENT_PLAY_CHANCE = 5e-5;

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
