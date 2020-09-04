#include "gamescene.h"
#include "gui/guieventfilter.h"
#include "../../controller/worldcontroller.h"
#include "../../model/registry.h"
#include "../../model/world.h"
#include "../../utils/askeyvaluerange.h"
#include "../../utils/consts.h"
#include "../../utils/glhelper.h"
#include "../../utils/experiencehelper.h"
#include "gui/inventorygui.h"
#include <QApplication>
#include <QEasingCurve>
#include <QtMath>

namespace parkour {

void GameScene::loadTexture() {
	// load block texture
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
	// load background image
	backgroundImg.load(":/assets/gui/bg.png");
	// load widgets texture
	widgetImg.load(":/assets/gui/widgets.png");
}

void GameScene::drawBackground(QPainter &p) {
	if (backgroundImg.isNull()) {
		return;
	}
	const float SPEED_FACTOR = 1.5f;
	auto viewportXMinTimesFactor = cameraInfo.getXMinOfViewport() * SPEED_FACTOR;
	float scaleRatio = p.device()->height() * 1.0f / backgroundImg.height();
	auto imageSizeOnScreen = QSize( backgroundImg.width(), backgroundImg.height() ) * scaleRatio;
	if (viewportXMinTimesFactor < 0) {
		int k = static_cast<int>(-viewportXMinTimesFactor) / imageSizeOnScreen.width();
		viewportXMinTimesFactor += (k + 2) * imageSizeOnScreen.width();
	}
	int start = -viewportXMinTimesFactor + static_cast<int>(viewportXMinTimesFactor) / backgroundImg.width();
	for (; start <= p.device()->width(); start += imageSizeOnScreen.width()) {
		p.drawImage(QRect(QPoint{ start, 0 }, imageSizeOnScreen), backgroundImg);
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

void GameScene::repaintEntityInfo(QPainter & p, const QPoint & bottomLeftOnScreen, QSharedPointer<Entity> entity) {
	QRect heartBackgroundSourceRect(QPoint(1, 117), QSize(9, 9)),
		  fullHeartSourceRect(QPoint(37, 117), QSize(9, 9)),
		  halfHeartSourceRect(QPoint(46, 117), QSize(9, 9));
	double ratio = (0.01 * deviceWidth) / heartBackgroundSourceRect.width();
	int margin = 1 * ratio;
	int totalHeartCount = qFloor(entity->getMaxHp()) / 2,
		filledHeartCount = qFloor(entity->getHp()) / 2;
	bool hasHalfHeart = qFloor(entity->getHp()) % 2 != 0;
	int lines = totalHeartCount % 10;

	int heartIndex = 0;
	for (int j = bottomLeftOnScreen.y(); ; j -= (margin + heartBackgroundSourceRect.height() * ratio)) {
		for (int k = 0; k < 10; k++) {
			int i = bottomLeftOnScreen.x() + k * (margin + heartBackgroundSourceRect.width() * ratio);
			if (heartIndex >= totalHeartCount) {
				return;
			}

			p.drawImage(QRect(QPoint(i, j - heartBackgroundSourceRect.height() * ratio), heartBackgroundSourceRect.size() * ratio), widgetImg, heartBackgroundSourceRect);
			if (heartIndex < filledHeartCount) {
				p.drawImage(QRect(QPoint(i, j - fullHeartSourceRect.height() * ratio), fullHeartSourceRect.size() * ratio), widgetImg, fullHeartSourceRect);
			} else if (heartIndex == filledHeartCount && hasHalfHeart) {
				p.drawImage(QRect(QPoint(i, j - halfHeartSourceRect.height() * ratio), halfHeartSourceRect.size() * ratio), widgetImg, halfHeartSourceRect);
			}

			heartIndex++;
		}
	}
}

void GameScene::repaintWorld(QPainter& p, QOpenGLContext& ctx) {


	const double marginTop = p.device()->height() - WORLD_HEIGHT * blockSizeOnScreen;
	// 把留白放置于上方
	p.translate(0, marginTop);

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

	const int PRELOAD_BLOCK_COUNT = 5;
    for (int i = cameraInfo.getXMinOfViewport() / blockSizeOnScreen - PRELOAD_BLOCK_COUNT;
         i <= (cameraInfo.getXMinOfViewport() + deviceWidth) / blockSizeOnScreen + PRELOAD_BLOCK_COUNT;
         i++) {
        for (int j = 0; j < WORLD_HEIGHT; j++) {
            auto block = blockRegistry.getBlockByName(world.getBlock(QPoint(i, j)));
            if (block == nullptr) {
                continue; // air block
            }

            auto blockId = static_cast<int>(blockRegistry.getBlockIdByName(block->getName()));
            int currentBlockTextureCount = this->blockTextureCount.at(blockId);
            int currentFrame = block->getCurrentFrame({ i, j });
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
        const auto& position = entity->getPosition();
		const auto& velocity = entity->getVelocity();
        const auto& texBox = entity->getTextureDimensions();
        if (!shouldRenderObject(position.x(), position.x() + texBox.x())) {
            continue;
        }
		const auto &uvRect = entity->getTextureRenderRect(); 
		// texture box
		auto targetTextureRect = QRectF(QPointF(position.x(), position.y()) * blockSizeOnScreen, QSizeF(texBox.x(), texBox.y()) * blockSizeOnScreen);
		const auto &entityTexture = getEntityTextureForPath(entity->getResourceLocation());
		if (uvRect.isNull()) {
			p.drawImage(targetTextureRect, entityTexture);
		} else {
			p.drawImage(targetTextureRect, entityTexture, uvRect);
		}
        
		if (entity->showDeathAnimationAndInfo()) {
			repaintEntityInfo(p, targetTextureRect.topLeft().toPoint(), entity);
		}
    }

    // 渲染濒临死亡的实体

    for (const auto& [entity, ticksLeft] : asKeyValueRange(dyingEntities)) {
        const auto& position = entity->getPosition();
        const auto& texBox = entity->getTextureDimensions();
        if (!shouldRenderObject(position.x(), position.x() + texBox.x()) || !entity->showDeathAnimationAndInfo()) {
            continue;
        }
        // texture box
        auto targetTextureRect = QRectF(QPointF(position.x(), position.y()) * blockSizeOnScreen, QSizeF(texBox.x(), texBox.y()) * blockSizeOnScreen);
		const auto &entityTexture = getEntityTextureForPath(entity->getResourceLocation());
		const auto &uvRect = entity->getTextureRenderRect();
        QTransform transform;
        transform.translate(targetTextureRect.right(), targetTextureRect.bottom());
        transform.rotate(90 * (DYING_ANIMATION_TICKS - ticksLeft) / DYING_ANIMATION_TICKS);

        p.setTransform(transform, true);
		auto transformedRect = QRectF(QPointF(-targetTextureRect.width(), -targetTextureRect.height()), \
			targetTextureRect.size());
		if (uvRect.isNull()) {
			p.drawImage(transformedRect, entityTexture);
		} else {
			p.drawImage(transformedRect, entityTexture, uvRect);
		}
        p.setTransform(transform.inverted(), true);
    }

	// 撤销留白
	p.translate(0, -marginTop);
}

void GameScene::repaintHud(QPainter& p, QOpenGLContext& ctx) {
    Q_UNUSED(ctx)

    auto playerController = WorldController::instance().getPlayerController();
	if (mode == GameScene::MAPEDIT) {
		auto player = playerController->getPlayer();
		if (player != nullptr) {
			QRect hotbarSourceRect(QPoint(1, 11), QSize(180, 20));
			const double ratio = 0.4 * deviceWidth / hotbarSourceRect.width();
			QPoint hotbarTargetStart;

			// draw hotbar texture
			hotbarTargetStart.setX(deviceWidth / 2 - hotbarSourceRect.width() * ratio / 2);
			hotbarTargetStart.setY(deviceHeight - hotbarSourceRect.height() * ratio);
			hotbarTargetRect = QRect(hotbarTargetStart, hotbarSourceRect.size() * ratio);
			p.drawImage(hotbarTargetRect, widgetImg, hotbarSourceRect);

			// draw hotbar overlay
			QRect hotbarOverlaySourceRect(QPoint(0, 33), QSize(24, 22));
			QPoint hotbarOverlayTargetStart;
			hotbarOverlayTargetStart.setX(hotbarTargetStart.x() - 2 * ratio + 20 * hotbarIndex * ratio);
			hotbarOverlayTargetStart.setY(hotbarTargetStart.y());
			p.drawImage(QRect(hotbarOverlayTargetStart, hotbarOverlaySourceRect.size() * ratio), widgetImg, hotbarOverlaySourceRect);

			// draw hotbar items
			QSharedPointer<Item> *inventory = player->getInventory();
			for (int i = 0; i < 9; i++) {
				if (inventory[3 * 9 + i] == nullptr) {
					continue;
				}
				int delta = 20 * i * ratio;
				QPoint itemTargetStart(hotbarTargetStart.x() + 3 * ratio + 20 * i * ratio, hotbarTargetStart.y() + 3 * ratio);
				auto itemTargetSize = QSize(15, 15) * ratio;
				p.drawImage(QRect(itemTargetStart, itemTargetSize), inventory[3 * 9 + i]->getIcon());
			}

			// draw item name
			if ((--hotbarTicksLeft) >= 0) {
				const int fontSize = 14.0 / 854.0 * deviceWidth, marginBottom = 5 / 854.0 * deviceWidth;
				auto item = inventory[3 * 9 + hotbarIndex];
				if (item != nullptr) {
					QFont simsun("宋体");
					simsun.setPixelSize(fontSize);
					p.setFont(simsun);
					const int alpha = 255 * (QEasingCurve(QEasingCurve::OutCubic).valueForProgress(hotbarTicksLeft * 1.0 / HOTBAR_TIMEOUT));
					p.setPen(QColor(255, 255, 255, alpha));
					p.drawText(QRect(hotbarTargetStart - QPoint(0, fontSize + marginBottom),
						QSize(hotbarTargetRect.size().width(), fontSize)), item->getDisplayName(), Qt::AlignCenter | Qt::AlignHCenter);
				}
			}
		}
	} else {
		// 游戏模式，渲染经验条和血量
		auto player = playerController->getPlayer();
		if (player != nullptr) {
			auto [level, xpLeft, xpRequired] = ExperienceHelper(player->getExp()).toLevel();
			QRect xpBackgroundSourceRect(QPoint(0, 0), QSize(182, 5));
			QRect xpValueSourceRect(QPoint(0, 5), QSize(xpLeft * 1.0f / xpRequired * 182.0, 5));
			const double ratio = 0.4 * deviceWidth / xpBackgroundSourceRect.width();
			QPoint xpTargetTopLeft((deviceWidth - xpBackgroundSourceRect.width() * ratio) * 0.5, 5 * ratio);
			p.drawImage(QRect(xpTargetTopLeft, xpBackgroundSourceRect.size() * ratio), widgetImg, xpBackgroundSourceRect);
			p.drawImage(QRect(xpTargetTopLeft, xpValueSourceRect.size() * ratio), widgetImg, xpValueSourceRect);

			QFont arial("Arial");
			const int fontSize = 7 * ratio;
			arial.setPixelSize(fontSize);
			p.setFont(arial);
			p.setPen(QColor(44, 196, 27)); 
			p.drawText(QRect(xpTargetTopLeft + QPoint(0, (xpBackgroundSourceRect.height() + 1) * ratio), 
							QSize(xpBackgroundSourceRect.width() * ratio, fontSize)),
				Qt::AlignCenter | Qt::AlignHCenter,
				QString::number(level));
		}
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

void GameScene::switchHotbar(int target) {
	hotbarIndex = target;
	hotbarTicksLeft = HOTBAR_TIMEOUT;
}

GameScene::GameScene(QObject* parent)
    : IScene(parent)
    , glBlockTexture(QOpenGLTexture::Target2D)
    , blockTextureImg(TEXTURE_MAP_SIZE, TEXTURE_MAP_SIZE, QImage::Format::Format_RGBA8888) {
    qDebug() << "loading test world...";
	auto &world = WorldController::instance();
    world.loadTestWorld();
    qDebug() << "loading texture...";
    loadTexture();
}

void GameScene::calculate() {
	if (mode == GameScene::GAMING) {
		WorldController::instance().tick();
	}
}

void GameScene::repaint(QPainter& p, QOpenGLContext& ctx) {

	deviceWidth = p.device()->width();
	deviceHeight = p.device()->height();
    blockSizeOnScreen = deviceHeight / BLOCK_TEXTURE_SIZE; // 屏幕坐标 / 游戏坐标
	Q_UNUSED(ctx)

	drawBackground(p);
	if (!cameraInfo.isMoving() && mode == SceneMode::GAMING) {
		moveViewportToPlayerPosition();
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

	// 渲染GUI
	if (ingameGUI != nullptr) {
		p.fillRect(0, 0, deviceWidth, deviceHeight, QColor(0, 0, 0, 200));
		ingameGUI->paintGUI(p);
	}
}

void GameScene::moveViewportToPlayerPosition() {
	auto playerController = WorldController::instance().getPlayerController();
	if (playerController->isAlive()) {
		auto player = playerController->getPlayer();
		const float playerXMin = player->getPosition().x() * blockSizeOnScreen;
		const float playerXMax = playerXMin + player->getTextureDimensions().x() * blockSizeOnScreen;
		const float xMinOfViewport = cameraInfo.getXMinOfViewport();
		const float xMaxOfViewport = xMinOfViewport + deviceWidth;
		const float screenEdgeOuter = SCREEN_EDGE_OUTER_WIDTH_MULTIPLIER * deviceWidth;
		const float screenEdgeInner = SCREEN_EDGE_INNER_WIDTH_MULTIPLIER * deviceWidth;

		if (playerXMax < xMinOfViewport + screenEdgeOuter) { // 玩家在屏幕规定范围左方
			cameraInfo.moveCameraTo(playerXMin - screenEdgeInner, xMinOfViewport - playerXMax > deviceWidth / 2);
		} else if (playerXMin > xMaxOfViewport - screenEdgeOuter) { // 玩家在屏幕规定范围右方
			cameraInfo.moveCameraTo(playerXMax - deviceWidth + screenEdgeInner, playerXMin - xMaxOfViewport > deviceWidth / 2);
		}
	}
}

bool GameScene::event(QEvent* event) {
	if (mode == GAMING) {
		auto playerController = WorldController::instance().getPlayerController();
		if (event->type() == QEvent::KeyPress) {
			const auto key = static_cast<QKeyEvent*>(event)->key();
			switch (key) {
			case Qt::Key_A:
				playerController->setGoingLeft(true);
				break;
			case Qt::Key_S:
			case Qt::Key_Shift:
				playerController->setSneakingExpected(true);
				break;
			case Qt::Key_D:
				playerController->setGoingRight(true);
				break;
			case Qt::Key_W:
				playerController->setReadyJump(true);
				break;
			case Qt::Key_O:
				WorldController::instance().saveWorld();
				break;
			case Qt::Key_P:
				WorldController::instance().loadWorld();
				break;
			case Qt::Key_C:
				mode = SceneMode::MAPEDIT;
				break;
			}
		} else if (event->type() == QEvent::KeyRelease) {
			const auto key = static_cast<QKeyEvent*>(event)->key();
			switch (key) {
			case Qt::Key_A:
				playerController->setGoingLeft(false);
				break;
			case Qt::Key_S:
			case Qt::Key_Shift:
				playerController->setSneakingExpected(false);
				break;
			case Qt::Key_D:
				playerController->setGoingRight(false);
				break;
			case Qt::Key_W:
				playerController->setReadyJump(false);
				break;
			}
		} else if (event->type() == QEvent::MouseButtonPress) {
			const auto mouseEvent = static_cast<QMouseEvent*>(event);
			if (mouseEvent->button() == Qt::MouseButton::LeftButton) {
				// 计算点击位置的游戏坐标
				int x = mouseEvent->x(), y = mouseEvent->y();
				const auto gameCoords = QVector2D(x + cameraInfo.getXMinOfViewport(), y) / blockSizeOnScreen;
				playerController->shootFireballAt(gameCoords);
			}
		}
		return true;
	} else {
		// 地图编辑模式
		if (ingameGUI == nullptr) {
			static QSharedPointer<GUIEventFilter> guiEventFilter;
			if (event->type() == QEvent::KeyRelease) {
				const auto key = static_cast<QKeyEvent*>(event)->key();
				switch (key) {
				case Qt::Key_A:
					cameraInfo.moveCameraTo(cameraInfo.getXMinOfViewport() - 0.5 * deviceWidth);
					break;
				case Qt::Key_D:
					cameraInfo.moveCameraTo(cameraInfo.getXMinOfViewport() + 0.5 * deviceWidth);
					break;
				}
			} else if (event->type() == QEvent::KeyPress) {
				const auto key = static_cast<QKeyEvent*>(event)->key();
				switch (key) {
				case Qt::Key_E: // inventory
					ingameGUI = QSharedPointer<InventoryGUI>::create();
					guiEventFilter = QSharedPointer<GUIEventFilter>::create(*this, Qt::Key_E);
					ingameGUI->installEventFilter(guiEventFilter.data());
					break;
				case Qt::Key_C:
					mode = SceneMode::GAMING;
					break;
#define HANDLE_HOTBAR(x) case Qt::Key_##x: switchHotbar(x-1); break;
				HANDLE_HOTBAR(1)
				HANDLE_HOTBAR(2)
				HANDLE_HOTBAR(3)
				HANDLE_HOTBAR(4)
				HANDLE_HOTBAR(5)
				HANDLE_HOTBAR(6)
				HANDLE_HOTBAR(7)
				HANDLE_HOTBAR(8)
				HANDLE_HOTBAR(9)
#undef HANDLE_HOTBAR
				}
			} else if (event->type() == QEvent::Wheel) {
				const int HOTBAR_SIZE = 9;
				const auto deg = static_cast<QWheelEvent*>(event)->angleDelta() / 8;
				const int movement = -deg.y() / 15;
				switchHotbar((hotbarIndex + (movement % HOTBAR_SIZE) + HOTBAR_SIZE) % HOTBAR_SIZE);
			} else if (event->type() == QEvent::MouseButtonPress) {
				const auto mouseEvent = static_cast<QMouseEvent*>(event);
				int screenX = mouseEvent->x(), screenY = mouseEvent->y();
				// 计算点击位置的游戏坐标
				const auto gameCoords = QVector2D(screenX + cameraInfo.getXMinOfViewport(), screenY) / blockSizeOnScreen;
				QPoint blockPos{ qFloor(gameCoords.x()), qFloor(gameCoords.y()) };
				auto &world = World::instance();
				auto player = WorldController::instance().getPlayerController()->getPlayer();
				if (!hotbarTargetRect.contains(screenX, screenY)) {
					if (player != nullptr) {
						if (mouseEvent->button() == Qt::MouseButton::LeftButton) {
							bool found = false;
							for (auto &entity : world.getEntities()) {
								if (entity->getName() == "player") {
									continue;
								}
								auto bbox = entity->getBoundingBoxWorld();
								auto mouseBbox = BoundingBoxWorld(gameCoords, BoundingBox{ {.0f, .0f}, {1.0f, 1.0f} });
								if (BoundingBoxWorld::intersect(bbox, mouseBbox)) {
									found = true;
									world.removeEntity(entity);
									break;
								}
							}

							if (!found) {
								world.setBlock(blockPos, "air");
							}

						} else if (mouseEvent->button() == Qt::MouseButton::RightButton) {
							auto item = player->getInventory()[3 * 9 + hotbarIndex];
							if (item != nullptr) {
								item->onUse(gameCoords);
							}
						}
					}
					for (int i = blockPos.x() - 1; i <= blockPos.x() + 1; i++) {
						for (int j = blockPos.y() - 1; j <= blockPos.y() + 1; j++) {
							const auto &blockName = world.getBlock({ i, j });
							if (blockName != "air" && !registry::BlockRegistry::instance().getBlockByName(blockName)->canPlaceAt({ i, j })) {
								world.setBlock({ i, j }, "air");
							}
						}
					}
				} else {
					if (mouseEvent->button() == Qt::MouseButton::LeftButton || mouseEvent->button() == Qt::MouseButton::RightButton) {
						int widthPerSlot = hotbarTargetRect.width() / 9;
						switchHotbar(std::clamp((screenX - hotbarTargetRect.left()) / widthPerSlot, 0, 8));
					}
				}
			}
		} else {
			QApplication::sendEvent(ingameGUI.data(), event);
		}
		return true;
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

void GameScene::closeGUI() {
	qDebug("close gui");
	ingameGUI = nullptr;
	qDebug() << ingameGUI;
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
	static QEasingCurve ease(QEasingCurve::InOutCubic);
    if (movingTicksLeft <= 0) {
        return;
    } else {
		if (! farMove) {
			xMinOfViewport = cameraStart + (cameraEnd - cameraStart)
				* ease.valueForProgress((CAMERA_MOVE_TICKS - movingTicksLeft) * 1.0 / CAMERA_MOVE_TICKS);
		} else {
			//按照二次函数移动相机, a为二次项系数
			auto a = (cameraStart - cameraEnd) / (CAMERA_MOVE_TICKS * CAMERA_MOVE_TICKS);
			xMinOfViewport = a * movingTicksLeft * movingTicksLeft + cameraEnd;
		}
        movingTicksLeft--;
    }
}

void GameScene::CameraInfo::moveCameraTo(float target, bool farMove) {
	cameraStart = xMinOfViewport;
	cameraEnd = target;
	movingTicksLeft = CAMERA_MOVE_TICKS;
	this->farMove = farMove;
}

bool GameScene::CameraInfo::isMoving() {
    return movingTicksLeft > 0;
}
}
