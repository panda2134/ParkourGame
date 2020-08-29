#ifndef WORLDCONTROLLER_H
#define WORLDCONTROLLER_H

#include "../utils/singleton.h"
#include "playercontroller.h"
#include <QObject>
#include <QSharedPointer>
#include <QTimer>

namespace parkour {
class WorldController : public QObject, public Singleton<WorldController> {
    Q_OBJECT

    QSharedPointer<PlayerController> playerController;

public:
    WorldController();

    void loadTestWorld() const;
    void unloadWorld() const;

    void explode(QPoint center, double power) const;
    QSharedPointer<PlayerController> getPlayerController() const;

public slots:
    void tick() const;
signals:
};
}

#endif // WORLDCONTROLLER_H
