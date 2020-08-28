#include "gamescene.h"

namespace parkour {

void GameScene::loadTexture() {
    auto& blockRegistry = registry::BlockRegistry::instance();
    auto& blockIds = blockRegistry.getBlockIds();
    qDebug() << "initializing texture: " << BLOCK_TEXTURE_SIZE * blockIds.size() << BLOCK_TEXTURE_SIZE;
    texture = QImage(TEXTURE_MAP_SIZE, TEXTURE_MAP_SIZE, QImage::Format_ARGB32);
    texture.fill(Qt::black);
    if (texture.isNull()) {
        qDebug() << "failed to initialize texture";
    }
    QPainter texturePainter(&(this->texture));
    textureCount.resize(1);
    for (int i = 1; i < blockIds.size(); i++) {
        auto block = blockRegistry.getBlockByName(blockIds[i]);
        if (block == nullptr) {
            continue;
        }
        QImage textureBlock;
        if (!textureBlock.load(block->getResourceLocation())) {
            qDebug() << "failed to load texture of block " << block->getName() << ", fallback to Unknown Texture";
            textureBlock.load(":/assets/blocks/no_texture.png");
        }
        texturePainter.drawImage(QPoint(BLOCK_TEXTURE_SIZE * i, 0), textureBlock);
        textureCount.push_back(textureBlock.height() / textureBlock.width());
    }
}

bool GameScene::shouldRenderObject(float xMin, float xMax) {
    auto xMinOfViewport = cameraInfo.getXMinOfViewport();
    return xMax > xMinOfViewport / blockSizeOnScreen || xMin < (xMinOfViewport + deviceWidth) / blockSizeOnScreen;
}

void GameScene::repaintWorld(QPainter& p, QOpenGLContext& ctx) {
    Q_UNUSED(ctx)

    const int PRELOAD_BLOCK_COUNT = 5;

    auto& world = World::instance();
    auto& blockRegistry = registry::BlockRegistry::instance();
    auto playerController = LocalWorldController::instance().getPlayerController();
    // 渲染方块
    for (int i = cameraInfo.getXMinOfViewport() / blockSizeOnScreen - PRELOAD_BLOCK_COUNT;
         i <= (cameraInfo.getXMinOfViewport() + deviceWidth) / blockSizeOnScreen + PRELOAD_BLOCK_COUNT;
         i++) {
        for (int j = 0; j <= WORLD_HEIGHT; j++) {
            auto block = blockRegistry.getBlockByName(world.getBlock(QPoint(i, j)));
            if (block == nullptr) {
                continue; // air block
            }

            auto blockId = static_cast<int>(blockRegistry.getBlockIdByName(block->getName()));
            int blockTextureCount = textureCount.at(blockId);
            int currentFrame = world.getTicksFromBirth() / TICKS_PER_FRAME % blockTextureCount;
            Q_ASSERT(blockTextureCount > 0);

            QPoint targetTopLeft(i * blockSizeOnScreen, j * blockSizeOnScreen);
            QPoint sourceTopLeft(blockId * BLOCK_TEXTURE_SIZE, currentFrame * BLOCK_TEXTURE_SIZE);
            QRect targetRect(targetTopLeft, QSize(blockSizeOnScreen, blockSizeOnScreen));
            QRect sourceRect(sourceTopLeft, QSize(BLOCK_TEXTURE_SIZE, BLOCK_TEXTURE_SIZE));
            p.drawImage(targetRect, texture, sourceRect);
        }
    }

    // 渲染存活的实体
    // 先画hitbox / 外层box，之后会做具体贴图渲染
    auto& entities = world.getEntities();
    auto& dyingEntities = world.getDyingEntities();

    for (const auto& entity : entities) {
        const auto& bbox = entity->getBoundingBoxWorld();
        const auto& position = entity->getPosition();
        const auto& texBox = entity->getTextureDimensions();
        if (!shouldRenderObject(position.x(), position.x() + texBox.x())) {
            continue;
        }
        float x1 = bbox.getMinX(),
              x2 = bbox.getMaxX(),
              y1 = bbox.getMinY(),
              y2 = bbox.getMaxY();
        // hit box
        QRect hitboxRect(QPoint(x1 * blockSizeOnScreen, y1 * blockSizeOnScreen),
            QPoint(x2 * blockSizeOnScreen, y2 * blockSizeOnScreen));
        p.setPen(Qt::blue);
        p.drawRect(hitboxRect);
        // texture box
        auto texRect = QRectF(QPointF(position.x(), position.y()) * blockSizeOnScreen, QSizeF(texBox.x(), texBox.y()) * blockSizeOnScreen);
        p.setPen(Qt::red);
        p.drawRect(texRect);
        // hp? flying? pos? velocity?
        p.setPen(Qt::red);
        QFont font("Consolas");
        QString info;
        info = QString("%1 %2 %3 %4").arg(QString::number(entity->getHp()), QString(entity->isOnFloor() ? "floor" : "flying"), QString::number(position.x()), QString::number(position.y()));
        font.setPixelSize(10);
        p.setFont(font);
        p.drawText(QPoint(texRect.left(), texRect.top()), info);
    }

    // 渲染濒临死亡的实体

    for (const auto& [entity, ticksLeft] : asKeyValueRange(dyingEntities)) {
        const auto& position = entity->getPosition();
        const auto& texBox = entity->getTextureDimensions();
        if (!shouldRenderObject(position.x(), position.x() + texBox.x())) {
            continue;
        }
        // texture box
        auto texRect = QRectF(QPointF(position.x(), position.y()) * blockSizeOnScreen, QSizeF(texBox.x(), texBox.y()) * blockSizeOnScreen);
        p.setPen(Qt::red);

        QTransform transform;
        transform.translate(texRect.right(), texRect.bottom());
        transform.rotate(90 * (DYING_ANIMATION_TICKS - ticksLeft) / DYING_ANIMATION_TICKS);

        p.setTransform(transform);
        p.drawRect(QRectF(QPoint(-texRect.width(), -texRect.height()), texRect.size()));
        p.setTransform(transform.inverted());
    }
}

void GameScene::repaintHud(QPainter& p, QOpenGLContext& ctx) {
    Q_UNUSED(ctx)

    auto playerController = LocalWorldController::instance().getPlayerController();
    // 渲染玩家信息
    if (playerController->isAlive()) {
        float xMax = p.device()->width();
        auto player = playerController->getPlayer();
        auto position = player->getPosition(), velocity = player->getVelocity(), acceleration = player->getAcceleration();
        QFont font("Consolas");
        font.setPixelSize(16);
        p.setFont(font);
        p.drawText(xMax - 100, 0, 100, 20, Qt::AlignRight | Qt::AlignTop, QString("%1 %2").arg(QString::number(position[0], 'g', 4), QString::number(position[1], 'g', 4)));
        p.drawText(xMax - 100, 20, 100, 20, Qt::AlignRight | Qt::AlignTop, QString("%1 %2").arg(QString::number(velocity[0], 'g', 4), QString::number(velocity[1], 'g', 4)));
        p.drawText(xMax - 100, 40, 100, 20, Qt::AlignRight | Qt::AlignTop, QString("%1 %2").arg(QString::number(acceleration[0], 'g', 4), QString::number(acceleration[1], 'g', 4)));
    }
}

GameScene::GameScene(QObject* parent)
    : IScene(parent) {
    qDebug() << "loading test world...";
    LocalWorldController::instance().loadTestWorld();
    qDebug() << "loading texture...";
    loadTexture();
}

void GameScene::calculate() {
    LocalWorldController::instance().tick();
}

void GameScene::repaint(QPainter& p, QOpenGLContext& ctx) {
    auto playerController = LocalWorldController::instance().getPlayerController();

    blockSizeOnScreen = p.device()->height() / BLOCK_TEXTURE_SIZE; // 屏幕坐标 / 游戏坐标
    deviceWidth = p.device()->width();

    if (playerController->isAlive() && !cameraInfo.isMoving()) {
        auto player = playerController->getPlayer();
        const float playerXMin = player->getPosition().x() * blockSizeOnScreen;
        const float playerXMax = playerXMin + player->getTextureDimensions().x() * blockSizeOnScreen;
        const float xMinOfViewport = cameraInfo.getXMinOfViewport();
        const float xMaxOfViewport = xMinOfViewport + deviceWidth;
        const float screenEdgeOuter = SCREEN_EDGE_OUTER_WIDTH_MULTIPLIER * deviceWidth;
        const float screenEdgeInner = SCREEN_EDGE_INNER_WIDTH_MULTIPLIER * deviceWidth;

        if (playerXMax < xMinOfViewport + screenEdgeOuter) { // 玩家在屏幕规定范围左方
            cameraInfo.moveCameraTo(playerXMin - screenEdgeInner);
        } else if (playerXMin > xMaxOfViewport - screenEdgeOuter) { // 玩家在屏幕规定范围右方
            cameraInfo.moveCameraTo(playerXMax - deviceWidth + screenEdgeInner);
        }
    }

    // 计算一帧的视口改变，并且渲染此视口改变
    cameraInfo.updateViewport();
    p.translate(-cameraInfo.getXMinOfViewport(), 0);

    // 渲染世界
    repaintWorld(p, ctx);

    // 撤销视口改变
    p.translate(cameraInfo.getXMinOfViewport(), 0);

    // 渲染HUD
    repaintHud(p, ctx);
}

bool GameScene::event(QEvent* event) {
    auto playerController = LocalWorldController::instance().getPlayerController();
    if (event->type() == QEvent::KeyPress) {
        const auto key = dynamic_cast<QKeyEvent*>(event)->key();
        switch (key) {
        case Qt::Key_A:
            playerController->setGoingLeft(true);
            break;
        case Qt::Key_D:
            playerController->setGoingRight(true);
            break;
        case Qt::Key_W:
            playerController->setReadyJump(true);
            break;
        }
    } else if (event->type() == QEvent::KeyRelease) {
        const auto key = dynamic_cast<QKeyEvent*>(event)->key();
        switch (key) {
        case Qt::Key_A:
            playerController->setGoingLeft(false);
            break;
        case Qt::Key_D:
            playerController->setGoingRight(false);
            break;
        case Qt::Key_W:
            playerController->setReadyJump(false);
            break;
        }
    }
    return IScene::event(event);
}

float GameScene::CameraInfo::getXMinOfViewport() const {
    return xMinOfViewport;
}

GameScene::CameraInfo::CameraInfo()
    : xMinOfViewport(0)
    , cameraStart(0)
    , cameraEnd(0)
    , movingTicksLeft(0) {
}

void GameScene::CameraInfo::updateViewport() {
    if (movingTicksLeft <= 0) {
        return;
    } else {
        // 按照二次函数移动相机, a为二次项系数
        auto a = (cameraStart - cameraEnd) / (CAMERA_MOVE_TICKS * CAMERA_MOVE_TICKS);
        xMinOfViewport = a * movingTicksLeft * movingTicksLeft + cameraEnd;
        movingTicksLeft--;
    }
}

void GameScene::CameraInfo::moveCameraTo(float target) {
    qDebug() << "move camera to" << target;
    movingTicksLeft = CAMERA_MOVE_TICKS;
    cameraStart = xMinOfViewport;
    cameraEnd = target;
}

bool GameScene::CameraInfo::isMoving() {
    return movingTicksLeft > 0;
}
}
