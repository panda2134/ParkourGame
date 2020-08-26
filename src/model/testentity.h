#ifndef TESTENTITY_H
#define TESTENTITY_H

#include "./boundingbox.h"
#include "./entity.h"
#include <QDebug>
#include <QVector2D>

namespace parkour {

class TestEntity : public Entity {
    Q_OBJECT
public:
    QString getName() const override;
    QString getResourceLocation() override;
    TestEntity() = default;
    TestEntity(QVector2D off, QVector2D dim)
        : offset(off)
        , dimensions(dim) {}

    QVector2D offset, dimensions;

    BoundingBox getBoundingBox() const;
    BoundingBoxWorld getBoundingBoxWorld() const override;

    void collide(const ICollidable&, Direction) override;
};
}

#endif // TESTENTITY_H
