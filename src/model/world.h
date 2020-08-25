#ifndef WORLD_H
#define WORLD_H

#include "../utils/singleton.h"
#include <QObject>

namespace parkour {
    class World : public QObject, public Singleton<World>
    {
        Q_OBJECT
        bool ready = false;
    public:
        bool getReady() const;
        void setReady(bool value);
    };
}

#endif // WORLD_H
