#include "gamerenderglwidget.h"
using namespace parkour;

GameRenderGLWidget::GameRenderGLWidget(QWidget* parent)
    : QOpenGLWidget(parent)
    , lastUpdateTime(std::chrono::high_resolution_clock::now())
    , worldController(LocalWorldController::instance()) {
    qDebug() << "loading test world...";
    worldController.loadTestWorld();
    qDebug() << "loading texture...";
    loadTexture();

    timer.setInterval(parkour::TICK_LENGTH * 1000);
    connect(&timer, SIGNAL(timeout()), this, SLOT(tick()));
    timer.start();
}

void GameRenderGLWidget::tick() {
    //    qDebug() << "ticking: start update in WorldController";
    worldController.tick();
    //    qDebug() << "ticking: start update in rendering";
    if (World::instance().getTicksFromBirth() % TICKS_PER_FRAME != 0)
        return;
    this->update();
    //    qDebug() << "ticking done";
}

void GameRenderGLWidget::paintFps(QPainter& p) {
    std::chrono::duration<double, std::milli> interval = (std::chrono::high_resolution_clock::now() - this->lastUpdateTime);
    p.setPen(Qt::yellow);
    auto fps = qRound(1000.0 / interval.count());
    QFont font("Consolas");
    font.setPixelSize(20);
    p.setFont(font);
    p.drawText(QPoint(0, 20), QString::number(fps));
}

void GameRenderGLWidget::loadTexture() {
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

// todo: switch to QGraphicsView for different scenes
void GameRenderGLWidget::paintGL() {
    QPainter p(this);

    // clear screen
    p.beginNativePainting();
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    p.endNativePainting();

    auto& world = World::instance();
    auto& blockRegistry = registry::BlockRegistry::instance();

    // 渲染方块
    for (int i = 0; i <= 30; i++) {
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
        float x1 = bbox.getMinX(),
              x2 = bbox.getMaxX(),
              y1 = bbox.getMinY(),
              y2 = bbox.getMaxY();
        // hit box
        QRect hitboxRect(QPoint(x1 * blockSizeOnScreen, y1 * blockSizeOnScreen),
            QPoint(x2 * blockSizeOnScreen, y2 * blockSizeOnScreen));
        p.setPen(Qt::blue);
        p.drawRect(hitboxRect);
        const auto& texBox = entity->getTextureDimensions();
        // texture box
        auto texRect = QRectF(QPointF(entity->getPosition().x(), entity->getPosition().y()) * blockSizeOnScreen,
            QSizeF(texBox.x(), texBox.y()) * blockSizeOnScreen);
        p.setPen(Qt::red);
        p.drawRect(texRect);
        // hp? flying?
        p.setPen(Qt::red);
        QFont font("Consolas");
        font.setPixelSize(20);
        p.setFont(font);
        p.drawText(QPoint(texRect.left(), texRect.top()), QString("%1 %2").arg(QString::number(entity->getHp()), QString(entity->isOnFloor() ? "floor" : "flying")));
    }

    // 渲染濒临死亡的实体

    for (const auto& [entity, ticksLeft] : asKeyValueRange(dyingEntities)) {
        const auto& texBox = entity->getTextureDimensions();
        // texture box
        auto texRect = QRectF(QPointF(entity->getPosition().x(), entity->getPosition().y()) * blockSizeOnScreen,
            QSizeF(texBox.x(), texBox.y()) * blockSizeOnScreen);
        p.setPen(Qt::red);

        QTransform transform;
        transform.translate(texRect.right(), texRect.bottom());
        transform.rotate(90 * (DYING_ANIMATION_TICKS - ticksLeft) / DYING_ANIMATION_TICKS);

        p.setTransform(transform);
        p.drawRect(QRectF(QPoint(-texRect.width(), -texRect.height()), texRect.size()));
        p.setTransform(transform.inverted());
    }

    paintFps(p);
    this->lastUpdateTime = std::chrono::high_resolution_clock::now();
}

void GameRenderGLWidget::initializeGL() {
    initializeOpenGLFunctions();
}
