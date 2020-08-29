#ifndef TESTENTITY_H
#define TESTENTITY_H

#include "./entityplayerlike.h"
#include <QDebug>
#include <QVector2D>

namespace parkour {

class TestEntity : public EntityPlayerLike {
    Q_OBJECT
public:
    QString getName() const override;
    QString getResourceLocation() override;
    QVector2D getTextureDimensions() override;
    TestEntity() = default;

    QVector2D offset, dimensions;

    BoundingBox getBoundingBox() const override;

    void update() override;
};
}

#endif // TESTENTITY_H
