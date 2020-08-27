#ifndef TESTENTITY_H
#define TESTENTITY_H

#include "../controller/localworldcontroller.h"
#include "./boundingbox.h"
#include "./playerlikeentity.h"
#include "world.h"
#include <QDebug>
#include <QVector2D>

namespace parkour {

class TestEntity : public PlayerLikeEntity {
    Q_OBJECT
public:
    QString getName() const override;
    QString getResourceLocation() override;
    QVector2D getTextureDimensions() override;
    TestEntity() = default;

    QVector2D offset, dimensions;

    BoundingBox getBoundingBox() const;
    BoundingBoxWorld getBoundingBoxWorld() const override;

    void update() override;
    void collide(const ICollidable&, Direction) override;
};
}

#endif // TESTENTITY_H
