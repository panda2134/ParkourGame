#ifndef LOCALWORLDCONTROLLER_H
#define LOCALWORLDCONTROLLER_H

#include "../model/blockdelegate.h"
#include "../model/world.h"
#include "../utils/consts.h"
#include "../utils/singleton.h"
#include <QObject>
#include <QSharedPointer>
#include <QTimer>

namespace parkour {
class LocalWorldController : public QObject, Singleton<LocalWorldController> {
    Q_OBJECT
    QTimer timer;

private:
    explicit LocalWorldController(QObject* parent = nullptr);

public:
    void loadTestWorld();
public slots:
    void tick();
signals:
};
}

#endif // LOCALWORLDCONTROLLER_H
