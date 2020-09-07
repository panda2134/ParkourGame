#include "worldioworker.h"
#include "savemanager.h"
#include <QDebug>

namespace parkour {
	QThread* WorldIOWorker::th = new QThread;

	void WorldIOWorker::setProgress(int value) {
		this->progressVal = value;
		emit progress(value);
	}
	void WorldIOWorker::runWorkerFunc() {
		workFunc();
		this->moveToThread(QApplication::instance()->thread());
		qDebug() << "done!";
		emit done();
	}
	void WorldIOWorker::executeWork(std::function<void()> func) {
		th->wait();
		qDebug("execute");
		auto &inst = WorldIOWorker::instance();
		inst.moveToThread(th);
		inst.workFunc = func;
		QApplication::instance()->connect(th, &QThread::started, &inst, &WorldIOWorker::runWorkerFunc);
		QApplication::instance()->connect(&inst, &WorldIOWorker::done, th, &QThread::quit);
		th->start();
	}
	int WorldIOWorker::getProgress() {
		return progressVal;
	}
}