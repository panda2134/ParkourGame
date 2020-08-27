#ifndef GAMERENDERGLWIDGET_H
#define GAMERENDERGLWIDGET_H

#include "../controller/localworldcontroller.h"
#include "../model/testentity.h"
#include "../utils/askeyvaluerange.h"
#include "../utils/consts.h"
#include "../utils/direction.h"
#include <QDebug>
#include <QElapsedTimer>
#include <QImage>
#include <QObject>
#include <QOpenGLContext>
#include <QOpenGLFunctions>
#include <QOpenGLWidget>
#include <QPaintEvent>
#include <QPainter>
#include <QRegion>
#include <QTime>
#include <QTimer>
#include <QVector2D>
#include <QWindow>
#include <chrono>

class GameRenderGLWidget : public QOpenGLWidget, protected QOpenGLFunctions {
    Q_OBJECT
    const double blockSizeOnScreen = 30.0f; // 游戏坐标 * 30.0 = 屏幕坐标
    QImage texture;
    QVector<int> textureCount;

    std::chrono::time_point<std::chrono::steady_clock> lastUpdateTime;

    void paintFps(QPainter&);
    void loadTexture();

public:
    GameRenderGLWidget(QWidget* parent = nullptr);

    QTimer timer;
    parkour::LocalWorldController& worldController;
public slots:
    void tick();

protected:
    void paintGL() override;
    void initializeGL() override;
};

#endif // GAMERENDERGLWIDGET_H
