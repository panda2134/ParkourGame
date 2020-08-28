#ifndef LOCALWORLDCONTROLLER_H
#define LOCALWORLDCONTROLLER_H

#include "../model/blockdelegate.h"
#include "../model/playerentity.h"
#include "../model/registry.h"
#include "../model/world.h"
#include "../utils/consts.h"
#include "../utils/singleton.h"
#include "playercontroller.h"
#include <QObject>
#include <QSharedPointer>
#include <QTimer>

namespace parkour {
class LocalWorldController : public QObject, public Singleton<LocalWorldController> {
    Q_OBJECT

    QSharedPointer<PlayerController> playerController;

public:
    LocalWorldController();

    void loadTestWorld() const;
    void unloadWorld() const;

    void explode(QPoint center, double power) const;
    QSharedPointer<PlayerController> getPlayerController() const;

public slots:
    void tick() const;
signals:
};
}

#endif // LOCALWORLDCONTROLLER_H
