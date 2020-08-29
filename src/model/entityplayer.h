#ifndef ENTITY_PLAYER_H
#define ENTITY_PLAYER_H

#include "./entityplayerlike.h"
#include <QVector2D>

namespace parkour {
class EntityPlayer : public EntityPlayerLike {
    Q_OBJECT
public:
    EntityPlayer();

public:
    BoundingBox getBoundingBox() const override;
    QString getName() const override;
    QString getResourceLocation() override;
    QVector2D getTextureDimensions() override;
};
}

#endif // ENTITY_PLAYER_H
