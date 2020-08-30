#include "gamescene.h"
#include "../../controller/worldcontroller.h"
#include "../../model/registry.h"
#include "../../model/world.h"
#include "../../utils/askeyvaluerange.h"
#include "../../utils/consts.h"
#include "../../utils/glhelper.h"

namespace parkour {

void GameScene::loadTexture() {
    auto& blockRegistry = registry::BlockRegistry::instance();
    auto& blockIds = blockRegistry.getBlockIds();
    qDebug() << "initializing texture: " << BLOCK_TEXTURE_SIZE * blockIds.size() << BLOCK_TEXTURE_SIZE;
	blockTextureImg.fill(0x00000000);
    if (blockTextureImg.isNull()) {
        qDebug() << "failed to initialize texture";
    }
    QPainter texturePainter(&blockTextureImg);
    blockTextureCount.resize(1);
    for (int i = 1; i < blockIds.size(); i++) {
        auto block = blockRegistry.getBlockByName(blockIds[i]);
        if (block == nullptr) {
            continue;
        }
        QImage textureBlock;
        if (!textureBlock.load(block->getResourceLocation())) {
            qDebug() << "failed to load texture of block " << block->getName() << ", fallback to Unknown Texture";
            textureBlock.load(NO_SUCH_TEXTURE);
        }
        texturePainter.drawImage(QPoint(BLOCK_TEXTURE_SIZE * i, 0), textureBlock);
        blockTextureCount.push_back(textureBlock.height() / textureBlock.width());
    }
}

void GameScene::writeQRectToOpenGLBuffer(QOpenGLBuffer& buf, const QRectF& rect) {
    float vertices[12];
    vertices[0] = rect.left(), vertices[1] = rect.bottom();
    vertices[2] = rect.left(), vertices[3] = rect.top();
    vertices[4] = rect.right(), vertices[5] = rect.bottom();
    vertices[6] = rect.left(), vertices[7] = rect.top();
    vertices[8] = rect.right(), vertices[9] = rect.top();
    vertices[10] = rect.right(), vertices[11] = rect.bottom();

    buf.bind();
    for (int i = 0; i < 6; i++) {
        const auto size = sizeof(float) * 2;
        const auto offset = i * size;
        buf.write(offset, &vertices[i * 2], size);
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
    auto playerController = WorldController::instance().getPlayerController();
    p.setPen(Qt::transparent);
    // 渲染方块
    p.beginNativePainting();
    auto glFuncs = ctx.functions();
    glFuncs->glEnable(GL_TEXTURE_2D);
    glFuncs->glEnable(GL_BLEND);
    glFuncs->glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glFuncs->glViewport(0, 0, p.device()->width(), p.device()->height());

    blockShader.bind();
    if (!blockShader.isLinked()) {
        blockShader.link();
    }
    vertexBuf.bind();
    blockShader.enableAttributeArray("a_vertPos");
    blockShader.setAttributeBuffer("a_vertPos", GL_FLOAT, 0, 2); // 一次读入2个float作为坐标

    blockTextureBuf.bind();
    blockShader.enableAttributeArray("a_texCoord");
    blockShader.setAttributeBuffer("a_texCoord", GL_FLOAT, 0, 2);

    double deviceWidth = p.device()->width(), deviceHeight = p.device()->height();
    QMatrix4x4 projectionMatrix(
        2 / deviceWidth, 0, 0, -1,
        0, -2 / deviceHeight, 0, 1,
        0, 0, 0, 0,
        0, 0, 0, 1);
    auto viewMatrix = projectionMatrix * QMatrix4x4(p.worldTransform());
    blockShader.setUniformValue("u_viewMatrix", viewMatrix);

    blockShader.setUniformValue("u_texWidth", blockTextureImg.width());
    blockShader.setUniformValue("u_texHeight", blockTextureImg.height());

    glFuncs->glDisable(GL_MULTISAMPLE);
    glBlockTexture.setMinificationFilter(QOpenGLTexture::Nearest);
    glBlockTexture.setMagnificationFilter(QOpenGLTexture::Nearest);
    glBlockTexture.setWrapMode(QOpenGLTexture::ClampToEdge);
    for (int i = cameraInfo.getXMinOfViewport() / blockSizeOnScreen - PRELOAD_BLOCK_COUNT;
         i <= (cameraInfo.getXMinOfViewport() + deviceWidth) / blockSizeOnScreen + PRELOAD_BLOCK_COUNT;
         i++) {
        for (int j = 0; j <= WORLD_HEIGHT; j++) {
            auto block = blockRegistry.getBlockByName(world.getBlock(QPoint(i, j)));
            if (block == nullptr) {
                continue; // air block
            }

            auto blockId = static_cast<int>(blockRegistry.getBlockIdByName(block->getName()));
            int currentBlockTextureCount = this->blockTextureCount.at(blockId);
			int currentFrame = block->getCurrentFrame();
			if (currentFrame < 0 || currentFrame >= currentBlockTextureCount) {
				currentFrame = world.getTicksFromBirth() / TICKS_PER_FRAME % currentBlockTextureCount;
			}
            Q_ASSERT(currentBlockTextureCount > 0);

            QPointF targetTopLeft(i * blockSizeOnScreen, j * blockSizeOnScreen);
            QPointF sourceTopLeft(blockId * BLOCK_TEXTURE_SIZE + 0.001, currentFrame * BLOCK_TEXTURE_SIZE);
            QRectF targetRect(targetTopLeft, QSizeF(blockSizeOnScreen, blockSizeOnScreen));
            QRectF sourceRect(sourceTopLeft, QSizeF(BLOCK_TEXTURE_SIZE, BLOCK_TEXTURE_SIZE));

            writeQRectToOpenGLBuffer(vertexBuf, targetRect);
            writeQRectToOpenGLBuffer(blockTextureBuf, sourceRect);

            glFuncs->glActiveTexture(GL_TEXTURE0);
            blockShader.setUniformValue("u_tex", 0);
            glBlockTexture.bind();

            glFuncs->glDrawArrays(GL_TRIANGLES, 0, 6); // 画6个顶点
        }
    }
    blockTextureBuf.release();
    vertexBuf.release();
    blockShader.release();
    glBlockTexture.release();
    p.endNativePainting();

    // 渲染存活的实体
    auto& entities = world.getEntities();
    auto& dyingEntities = world.getDyingEntities();

    for (const auto& entity : entities) {
        const auto& bbox = entity->getBoundingBoxWorld();
        const auto& position = entity->getPosition();
		const auto& velocity = entity->getVelocity();
        const auto& texBox = entity->getTextureDimensions();
        if (!shouldRenderObject(position.x(), position.x() + texBox.x())) {
            continue;
        }
        float x1 = bbox.getMinX(),
              x2 = bbox.getMaxX(),
              y1 = bbox.getMinY(),
              y2 = bbox.getMaxY();
		// texture box
		auto texRect = QRectF(QPointF(position.x(), position.y()) * blockSizeOnScreen, QSizeF(texBox.x(), texBox.y()) * blockSizeOnScreen);
		p.drawImage(texRect, getEntityTextureForPath(entity->getResourceLocation()));
        // hit box
        QRect hitboxRect(QPoint(x1 * blockSizeOnScreen, y1 * blockSizeOnScreen),
            QPoint(x2 * blockSizeOnScreen, y2 * blockSizeOnScreen));
        p.setPen(Qt::blue);
        p.drawRect(hitboxRect);
        // hp? flying? pos? velocity?
        p.setPen(Qt::red);
        QFont font("Consolas");
        QString info;
        info = QString("%1 %2 %3 %4").arg(QString::number(entity->getHp()), QString(entity->isOnFloor() ? "floor" : "flying"), QString::number(velocity.x()), QString::number(velocity.y()));
        font.setPixelSize(10);
        p.setFont(font);
        p.drawText(QPoint(texRect.left(), texRect.top()), info);
    }

    // 渲染濒临死亡的实体

    for (const auto& [entity, ticksLeft] : asKeyValueRange(dyingEntities)) {
        const auto& position = entity->getPosition();
        const auto& texBox = entity->getTextureDimensions();
        if (!shouldRenderObject(position.x(), position.x() + texBox.x()) || !entity->showDeathAnimation()) {
            continue;
        }
        // texture box
        auto texRect = QRectF(QPointF(position.x(), position.y()) * blockSizeOnScreen, QSizeF(texBox.x(), texBox.y()) * blockSizeOnScreen);

        QTransform transform;
        transform.translate(texRect.right(), texRect.bottom());
        transform.rotate(90 * (DYING_ANIMATION_TICKS - ticksLeft) / DYING_ANIMATION_TICKS);

        p.setTransform(transform, true);
		p.drawImage(QRectF(QPointF(-texRect.width(), -texRect.height()), texRect.size()), getEntityTextureForPath(entity->getResourceLocation()));
        p.setTransform(transform.inverted(), true);
    }
}

void GameScene::repaintHud(QPainter& p, QOpenGLContext& ctx) {
    Q_UNUSED(ctx)

    auto playerController = WorldController::instance().getPlayerController();
    // 渲染玩家信息
    if (playerController->isAlive()) {
        float xMax = p.device()->width();
        auto player = playerController->getPlayer();
        auto position = player->getPosition(), velocity = player->getVelocity(), acceleration = player->getAcceleration();
        QFont font("Consolas");
        font.setPixelSize(16);
        p.setFont(font);
        p.drawText(xMax - 300, 0, 300, 20, Qt::AlignRight | Qt::AlignTop, QString("%1 %2").arg(QString::number(position[0], 'g', 4), QString::number(position[1], 'g', 4)));
        p.drawText(xMax - 300, 20, 300, 20, Qt::AlignRight | Qt::AlignTop, QString("%1 %2").arg(QString::number(velocity[0], 'g', 4), QString::number(velocity[1], 'g', 4)));
        p.drawText(xMax - 300, 40, 300, 20, Qt::AlignRight | Qt::AlignTop, QString("%1 %2").arg(QString::number(acceleration[0], 'g', 4), QString::number(acceleration[1], 'g', 4)));
    }
}

const QImage & GameScene::getEntityTextureForPath(const QString & path) {
	if (entityTextureCache.count(path) > 0) {
		return entityTextureCache[path];
	} else {
		auto image = QImage();
		if (!image.load(path)) { 
			image.load(NO_SUCH_TEXTURE);
		}
		return entityTextureCache[path] = image;
	}
}

GameScene::GameScene(QObject* parent)
    : IScene(parent)
    , glBlockTexture(QOpenGLTexture::Target2D)
    , blockTextureImg(TEXTURE_MAP_SIZE, TEXTURE_MAP_SIZE, QImage::Format::Format_RGBA8888) {
    qDebug() << "loading test world...";
    WorldController::instance().loadTestWorld();
    qDebug() << "loading texture...";
    loadTexture();
}

void GameScene::calculate() {
    WorldController::instance().tick();
}

void GameScene::repaint(QPainter& p, QOpenGLContext& ctx) {
    auto playerController = WorldController::instance().getPlayerController();

    blockSizeOnScreen = 30.0; // 屏幕坐标 / 游戏坐标
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
    const double marginTop = p.device()->height() - WORLD_HEIGHT * blockSizeOnScreen;
    // 把留白放置于上方
    p.translate(0, marginTop);

    // 渲染世界
    repaintWorld(p, ctx);

    // 撤销留白
    p.translate(0, -marginTop);
    // 撤销视口改变
    p.translate(cameraInfo.getXMinOfViewport(), 0);

    // 渲染HUD
    repaintHud(p, ctx);
}

bool GameScene::event(QEvent* event) {
    auto playerController = WorldController::instance().getPlayerController();
    if (event->type() == QEvent::KeyPress) {
        const auto key = dynamic_cast<QKeyEvent*>(event)->key();
        switch (key) {
        case Qt::Key_A:
            playerController->setGoingLeft(true);
            break;
        case Qt::Key_S:
            playerController->setSneakingExpected(true);
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
        case Qt::Key_S:
            playerController->setSneakingExpected(false);
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

void GameScene::initializeGL() {
    // 加载显卡上的材质
    if (!glBlockTexture.create()) {
        throw "Failed to initialize OpenGL Texture";
    }
    glBlockTexture.setSize(blockTextureImg.width(), blockTextureImg.height());
    // glBlockTexture.setFormat(QOpenGLTexture::RGBA8_UNorm);
    glBlockTexture.setWrapMode(QOpenGLTexture::ClampToEdge);
    glBlockTexture.setData(blockTextureImg, QOpenGLTexture::DontGenerateMipMaps);

    // 加载顶点缓冲
    vertexBuf.create();
    vertexBuf.setUsagePattern(QOpenGLBuffer::DynamicDraw);
    vertexBuf.bind();
    vertexBuf.allocate(2 * 3 * sizeof(float) * 2); // 2个三角形，对每个三角形有x, y坐标
    vertexBuf.release();
    // 加载材质顶点缓冲
    blockTextureBuf.create();
    blockTextureBuf.setUsagePattern(QOpenGLBuffer::DynamicDraw);
    blockTextureBuf.bind();
    blockTextureBuf.allocate(2 * 3 * sizeof(float) * 2);
    blockTextureBuf.release();

    // 加载方块着色器
    if (!blockShader.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/assets/shaders/block.vert")) {
        throw "failed to load block vertex shaders";
    }
    if (!blockShader.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/assets/shaders/block.frag")) {
        throw "failed to load block fragment shaders";
    }
    if (!blockShader.link()) {
        throw "failed to link shaders";
    }
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
    movingTicksLeft = CAMERA_MOVE_TICKS;
    cameraStart = xMinOfViewport;
    cameraEnd = target;
}

bool GameScene::CameraInfo::isMoving() {
    return movingTicksLeft > 0;
}
}
