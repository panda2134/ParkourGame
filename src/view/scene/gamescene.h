#ifndef GAMESCENE_H
#define GAMESCENE_H

#include "iscene.h"
#include <QHash>
#include <QByteArray>
#include <QEvent>
#include <QKeyEvent>
#include <QMatrix4x4>
#include <QMouseEvent>
#include <QObject>
#include <QOpenGLBuffer>
#include <QOpenGLDebugLogger>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>

namespace parkour {

class GameScene : public IScene {
    Q_OBJECT

    class CameraInfo {
        float xMinOfViewport;
        float cameraStart;
        float cameraEnd;
        int movingTicksLeft;

    public:
        CameraInfo();
        void updateViewport();
        void moveCameraTo(float target);
        bool isMoving();
        float getXMinOfViewport() const;
    };

    CameraInfo cameraInfo;
    QOpenGLBuffer vertexBuf, blockTextureBuf;
    QOpenGLShaderProgram blockShader;
    QOpenGLTexture glBlockTexture;
    QImage blockTextureImg;
	QVector<int> blockTextureCount;
	QHash<QString, QImage> entityTextureCache;
	const QString NO_SUCH_TEXTURE = ":/assets/blocks/no_texture.png";
    double blockSizeOnScreen = 0.0;
    double deviceWidth = 854.0;
    void loadTexture();
    void writeQRectToOpenGLBuffer(QOpenGLBuffer& buf, const QRectF& rect);
    /**
     * @brief shouldRenderObject 判断是否应该渲染某个对象
     * @param xMin 游戏坐标系里 x 的最小值
     * @param xMax 游戏坐标系里 x 的最大值
     * @return 是否渲染
     */
    bool shouldRenderObject(float xMin, float xMax);

    void repaintWorld(QPainter&, QOpenGLContext&);
    void repaintHud(QPainter&, QOpenGLContext&);

	const QImage& getEntityTextureForPath(const QString &path);

public:
    explicit GameScene(QObject* parent = nullptr);
    void calculate() override;
    void repaint(QPainter&, QOpenGLContext&) override;
    bool event(QEvent* evt) override;
    void initializeGL() override;
};
}

#endif // GAMESCENE_H
