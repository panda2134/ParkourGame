#ifndef GAMERENDERGLWIDGET_H
#define GAMERENDERGLWIDGET_H

#include "./scene/gamescene.h"
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
#include <QThread>
#include <chrono>

class GameRenderGLWidget : public QOpenGLWidget, protected QOpenGLFunctions {
    Q_OBJECT

    /**
     * @brief renderTick 渲染时世界不一定已加载，故渲染组件需要单独维护一个tick计数
     */
    size_t renderTick;
	QThread *timerThread;
    std::chrono::time_point<std::chrono::steady_clock> lastUpdateTime;

public:
	QSharedPointer<parkour::GameScene> gameScene;
    GameRenderGLWidget(QWidget* parent = nullptr);
	~GameRenderGLWidget();
public slots:
    void tick();

protected:
    bool event(QEvent* e) override;
    void resizeEvent(QResizeEvent* e) override;
    void paintGL() override;
    void initializeGL() override;
	void closeEvent(QCloseEvent *) override;
};

#endif // GAMERENDERGLWIDGET_H
