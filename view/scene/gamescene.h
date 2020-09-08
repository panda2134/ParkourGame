#ifndef GAMESCENE_H
#define GAMESCENE_H

#include "../../utils/consts.h"
#include "../../model/entity.h"
#include "gui/guibase.h"
#include <QHash>
#include <QByteArray>
#include <QEvent>
#include <QKeyEvent>
#include <QMatrix4x4>
#include <QMouseEvent>
#include <QSharedPointer>
#include <QObject>
#include <QOpenGLBuffer>
#include <QOpenGLDebugLogger>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>

namespace parkour {
	enum SceneMode {
		GAMING,
		MAPEDIT
	};
class GameScene : public QObject {
    Q_OBJECT

    struct CameraInfo {
        float xMinOfViewport;
        float cameraStart;
        float cameraEnd;
        int movingTicksLeft;
		bool farMove;

        CameraInfo();
        void updateViewport();
        void moveCameraTo(float target, bool farMove = false);
        bool isMoving();
        float getXMinOfViewport() const;
    };

	const QString NO_SUCH_TEXTURE = ":/assets/blocks/no_texture.png";
	const int HOTBAR_TIMEOUT = 1 * TICKS_PER_SEC / TICKS_PER_FRAME;
	bool quitting = false;

	int hotbarIndex = 0, hotbarTicksLeft = -1;
	QSharedPointer<GUIBase> ingameGUI;
    CameraInfo cameraInfo;
    QOpenGLBuffer vertexBuf, blockTextureBuf;
    QOpenGLShaderProgram blockShader;
    QOpenGLTexture glBlockTexture;
    QImage blockTextureImg, backgroundImg, widgetImg;
	QRect hotbarTargetRect;
	QVector<int> blockTextureCount;
	QHash<QString, QImage> entityTextureCache;
    double blockSizeOnScreen = 0.0;
    double deviceWidth = 854.0, deviceHeight = 480.0;
    void loadTexture();
	void drawBackground(QPainter &p);
    void writeQRectToOpenGLBuffer(QOpenGLBuffer& buf, const QRectF& rect);
    /**
     * @brief shouldRenderObject 判断是否应该渲染某个对象
     * @param xMin 游戏坐标系里 x 的最小值
     * @param xMax 游戏坐标系里 x 的最大值
     * @return 是否渲染
     */
    bool shouldRenderObject(float xMin, float xMax);
	void repaintEntityInfo(QPainter & p, const QPoint & bottomLeftOnScreen, QSharedPointer<Entity> entity);

    void repaintWorld(QPainter&, QOpenGLContext&);
    void repaintHud(QPainter&, QOpenGLContext&);

	const QImage& getEntityTextureForPath(const QString &path);

	void switchHotbar(int target);

public:
	SceneMode mode = GAMING;
    explicit GameScene(QObject* parent = nullptr);
    void calculate();
    void repaint(QPainter&, QOpenGLContext&);
	void moveViewportToPlayerPosition();
    void initializeGL();
	void showGUI(QSharedPointer<GUIBase> gui, Qt::Key exitKey);
	bool event(QEvent* evt) override;
	void setQuitting();


public slots:
	void quitGUI();
signals:
	void saveAndExit();
};
}

#endif // GAMESCENE_H
