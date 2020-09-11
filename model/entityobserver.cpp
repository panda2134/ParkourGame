#include "entityobserver.h"
#include "blocktnt.h"
#include "registry.h"
#include "world.h"

namespace parkour {
void EntityObserver::serializeCustomProps(QDataStream& out) const {
    out << playerFound;
}
void EntityObserver::deserializeCustomProps(QDataStream& in) {
    in >> playerFound;
}
int EntityObserver::getSerializationVersion() const {
    return 1;
}
QString EntityObserver::getName() const {
    return "observer";
}
QString EntityObserver::getResourceLocation() {
    return ":/assets/entities/observer.png";
}
QVector2D EntityObserver::getTextureDimensions() {
    return QVector2D(1, 1);
}
BoundingBox EntityObserver::getBoundingBox() const {
    return BoundingBox { { .0f, .0f }, { 1.0f, 1.0f } };
}
QString EntityObserver::getDisplayName() const {
    return "观察器";
}
QRect EntityObserver::getTextureRenderRect() const {
    return playerFound ? QRect(QPoint(0, 16), QSize(16, 16)) : QRect(QPoint(0, 0), QSize(16, 16));
}
void EntityObserver::update() {
    QSharedPointer<Entity> player;
    auto& world = World::instance();
    for (auto x : world.getEntities()) {
        if (x->getName() == "player") {
            player = x;
            break;
        }
    }
    if (player == nullptr) {
        return;
    }

    playerFound = (getPosition() - player->getPosition()).length() < DETECT_RANGE;

    if (playerFound) {
        QPoint pos = getPosition().toPoint();
        auto tnt = (registry::BlockRegistry::instance().getBlockByName("tnt")).staticCast<BlockTNT>();
        for (int i = pos.x() - IGNITE_RANGE; i <= pos.x() + IGNITE_RANGE; i++) {
            for (int j = pos.y() - IGNITE_RANGE; j <= pos.y() + IGNITE_RANGE; j++) {
                if (world.getBlock({ i, j }) == "tnt") {
                    tnt->ignite({ i, j });
                }
            }
        }
    }
}
bool EntityObserver::showDeathAnimationAndInfo() const {
    return false;
}

double EntityObserver::getMass() const {
    return 1e8;
}

}