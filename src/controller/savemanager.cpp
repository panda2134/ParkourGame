#include "savemanager.h"
#include "worldcontroller.h"
#include "../model/world.h"
#include <QStandardPaths>
#include <QDir>
#include <QFile>
#include <QDebug>
#include <QFileInfo>

namespace parkour {
	SaveManager::SaveManager() {
		savesFolder = QDir(QStandardPaths::standardLocations(QStandardPaths::AppLocalDataLocation)[0] + "/MineParkour");
		if (!savesFolder.exists()) {
			savesFolder.mkpath(".");
		}
	}
	
	QList<SaveManager::SaveMeta> SaveManager::listSaves() {
		QList<SaveMeta> ret;
		for (QString saveName : savesFolder.entryList(QDir::Dirs | QDir::NoDotAndDotDot)) {
			SaveMeta item;
			item.saveName = saveName;
			item.lastModifiedTime = QFileInfo(savesFolder.filePath(saveName + "/" + SAVE_FILENAME)).lastModified();
			item.saveImage = QImage(savesFolder.filePath(saveName + "/" + PREVIEW_FILENAME));
			ret.push_back(item);
		}
		return ret;
	}
	bool SaveManager::loadFromSave(const QString &saveName) {
		QFile saveFile(savesFolder.filePath(saveName + "/" + SAVE_FILENAME));
		if (saveFile.open(QIODevice::ReadOnly)) {
			QDataStream in(&saveFile);
			WorldController::instance().deserializeWorld(in);
			World::instance().setReady(true);
			return in.status() == QDataStream::Ok;
		} else {
			return false;
		}
	}
	bool SaveManager::writeToSave(const QString &saveName) {
		QFile saveFile(savesFolder.filePath(saveName + "/" + SAVE_FILENAME));
		//qDebug("41");
		if (saveFile.open(QIODevice::WriteOnly)) {
			QDataStream out(&saveFile);
			//qDebug("before serialize");
			WorldController::instance().serializeWorld(out);
			//qDebug("after serialize");
			lastScreenshot.save(savesFolder.filePath(saveName + "/" + PREVIEW_FILENAME));
			//qDebug("after preview");
			World::instance().setReady(false);
			//qDebug("before return");
			return out.status() == QDataStream::Ok;
		} else {
			return false;
		}
	}
	bool SaveManager::createSaveFolder(const QString &saveName) {
		return savesFolder.mkdir(saveName);
	}
	void SaveManager::setLastScreenshot(const QImage & im) {
		lastScreenshot = im;
	}
}