#ifndef TESTENTITY_H
#define TESTENTITY_H

#include "./boundingbox.h"
#include "./entity.h"
#include <QVector2D>

namespace parkour {

class TestEntity : public Entity
{
public:
    TestEntity() = default;

    BoundingBox getBoundingBox() const;
    BoundingBoxWorld getBoundingBoxWorld() const override;
};
}

#endif // TESTENTITY_H
