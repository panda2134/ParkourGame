#ifndef SAVEMANAGER_H
#define SAVEMANAGER_H

#include "../utils/singleton.h"
#include <QDateTime>
#include <QDir>
#include <QImage>
#include <QList>
#include <QObject>
#include <QString>

namespace parkour {

class SaveManager : public QObject, public Singleton<SaveManager> {
    Q_OBJECT

    QDir savesFolder;
    QImage lastScreenshot;

public:
    struct SaveMeta {
        QImage saveImage;
        QString saveName;
        QDateTime lastModifiedTime;
    };
    const char* PREVIEW_FILENAME = "preview.png";
    const char* SAVE_FILENAME = "save.dat";
    SaveManager();
    QList<SaveMeta> listSaves();
    bool loadFromSave(const QString&);
    bool writeToSave(const QString&);
    bool createSaveFolder(const QString&);
    void setLastScreenshot(const QImage& im);
    void removeSave(QString saveName);
};
}

#endif