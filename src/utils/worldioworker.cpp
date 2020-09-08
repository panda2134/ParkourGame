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
		QThread::msleep(500); // 保证进度条被显示
		emit progress(100);
		this->moveToThread(QApplication::instance()->thread());
		emit done();
	}
	void WorldIOWorker::executeWork(std::function<void()> func) {
		static bool connected = false;
		th->wait();
		auto &inst = WorldIOWorker::instance();
		inst.moveToThread(th);
		inst.workFunc = func;
		if (!connected) {
			connected = true;
			QApplication::instance()->connect(th, &QThread::started, &inst, &WorldIOWorker::runWorkerFunc);
			QApplication::instance()->connect(&inst, &WorldIOWorker::done, th, &QThread::quit);
		}
		th->start();
	}
	int WorldIOWorker::getProgress() {
		return progressVal;
	}
}