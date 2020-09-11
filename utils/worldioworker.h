#ifndef WORLDIOWORKER_H
#define WORLDIOWORKER_H

#include "controller/savemanager.h"
#include "utils/singleton.h"
#include <QApplication>
#include <QMutex>
#include <QObject>
#include <QThread>
#include <functional>

namespace parkour {
class WorldIOWorker : public QObject, public Singleton<WorldIOWorker> {
    Q_OBJECT
private:
    static QThread* th;
    std::function<void()> workFunc;
    int progressVal;
private slots:
    void runWorkerFunc();

public:
    static void executeWork(std::function<void()> func);
    int getProgress();

public slots:
    void setProgress(int);

signals:
    void progress(int);
    void done();
};
}

#endif
