#ifndef GAMESCENE_H
#define GAMESCENE_H

#include "../../controller/localworldcontroller.h"
#include "../../model/world.h"
#include "../../utils/askeyvaluerange.h"
#include "../../utils/consts.h"
#include "iscene.h"
#include <QEvent>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QObject>

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
    QImage texture;
    QVector<int> textureCount;
    double blockSizeOnScreen = 0.0;
    double deviceWidth = 854.0;
    void loadTexture();
    /**
     * @brief shouldRenderObject 判断是否应该渲染某个对象
     * @param xMin 游戏坐标系里 x 的最小值
     * @param xMax 游戏坐标系里 x 的最大值
     * @return 是否渲染
     */
    bool shouldRenderObject(float xMin, float xMax);

    void repaintWorld(QPainter&, QOpenGLContext&);
    void repaintHud(QPainter&, QOpenGLContext&);

public:
    explicit GameScene(QObject* parent = nullptr);
    void calculate() override;
    void repaint(QPainter&, QOpenGLContext&) override;
    bool event(QEvent* evt) override;
};
}

#endif // GAMESCENE_H
