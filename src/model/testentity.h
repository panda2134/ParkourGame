#ifndef TESTENTITY_H
#define TESTENTITY_H

#include "./boundingbox.h"
#include "./entity.h"
#include <QDebug>
#include <QVector2D>

namespace parkour {

class TestEntity : public Entity
{
public:
    TestEntity() = default;
    TestEntity(QVector2D dim)
        : dimensions(dim) {}

    QVector2D dimensions;

    BoundingBox getBoundingBox() const;
    BoundingBoxWorld getBoundingBoxWorld() const override;

    void collide(const ICollidable&, Direction) override;
};
}

#endif // TESTENTITY_H
