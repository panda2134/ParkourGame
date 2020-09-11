#include "itemspawnegg.h"
#include "../utils/direction.h"
#include "blockdelegate.h"
#include "boundingbox.h"
#include "entity.h"
#include "registry.h"
#include "world.h"
#include <QSharedPointer>
#include <QString>
#include <algorithm>

namespace parkour {
ItemSpawnEgg::ItemSpawnEgg(QString entityClassName_, QString iconPath_)
    : entityClassName(entityClassName_) {
    icon.load(iconPath_);
}
QString ItemSpawnEgg::getDisplayName() {
    auto entity = registry::EntityRegistry::instance().generateEntity(entityClassName);
    if (entity != nullptr) {
        return QString("生成 %1").arg(entity->getDisplayName());
    } else {
        return QString("无效刷怪蛋");
    }
}
const QImage& ItemSpawnEgg::getIcon() {
    return icon;
}
void ItemSpawnEgg::onUse(QVector2D clickPos) {
    auto entity = registry::EntityRegistry::instance().generateEntity(entityClassName);
    if (entity == nullptr) {
        return;
    }
    auto& world = World::instance();
    BoundingBox bbox = entity->getBoundingBox();
    entity->placeBoundingBoxAt(clickPos);
    // 检查是否和方块碰撞
    for (const auto& anotherEntity : world.getEntities()) {
        if (BoundingBoxWorld::intersect(entity->getBoundingBoxWorld(), anotherEntity->getBoundingBoxWorld())) {
            qDebug() << "cannot spawn: collide with entity";
            return;
        }
    }
    // 检查是否和实体碰撞
    int range = std::max({ ENTITY_COLLISION_RANGE, 1.0 * bbox.dimensions.x(), 1.0 * bbox.dimensions.y() }) + 1;
    for (int i = clickPos[0] - range; i <= clickPos[0] + range; i++) {
        for (int j = clickPos[1] - range; j <= clickPos[1] + range; j++) {
            auto blockDelegate = BlockDelegate({ i, j });

            if (!blockDelegate.isNull() && BoundingBoxWorld::intersect(entity->getBoundingBoxWorld(), blockDelegate.getBoundingBoxWorld())) {
                qDebug() << "cannot spawn: collide with block" << i << j;
                return;
            }
        }
    }
    // 无碰撞，直接加入
    world.addEntity(entity);
}
}