#ifndef GAMESOUND_H
#define GAMESOUND_H

#include "utils/singleton.h"
#include <QHash>
#include <QMediaPlayer>
#include <QObject>
#include <QSharedPointer>
#include <QSoundEffect>
#include <QString>
#include <QThread>
#include <QVector2D>

namespace parkour {
class GameSound : public QObject, public Singleton<GameSound> {
    Q_OBJECT

    QHash<QString, QSharedPointer<QMediaPlayer>> ambients;
    QHash<QString, QSharedPointer<QSoundEffect>> sounds;

    QSharedPointer<QMediaPlayer> currentAmbientSound;

    void loadSound(const QString&);

    class GameSoundWorker : public QObject {
        GameSound* parent;

    public:
        GameSoundWorker(GameSound* parent) {
            this->parent = parent;
        }
        void playSound(QString name);
        void playCurrentAmbient();
        void stopCurrentAmbient();
        void playWorldSound(QString name, QVector2D position);
    };

    GameSoundWorker* worker = nullptr;
    QThread th;

signals:
    void playSoundInWorker(QString);
    void playWorldSoundInWorker(QString, QVector2D);
    void playAmbientInWorker();
    void stopAmbientInWorker();

public:
    GameSound();
    ~GameSound();
    void playSound(const QString&);
    void playWorldSound(const QString&, const QVector2D&);
    void playAmbient();
    void stopAmbient();
};
}

#endif
