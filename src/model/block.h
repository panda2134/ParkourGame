#ifndef BLOCK_H
#define BLOCK_H

#include "../utils/types.h"
#include "./boundingbox.h"
#include "./entity.h"
#include <QObject>

namespace parkour {

class Block : public QObject {
    Q_OBJECT
public:
    explicit Block() {};

    virtual QString getName() const = 0;
    virtual QString getResourceLocation() const = 0;

    virtual void collide(const BlockPosType&, const Entity&, Direction) {};
    virtual BoundingBox getBoundingBox() const;
signals:
};

}

#endif // BLOCK_H
