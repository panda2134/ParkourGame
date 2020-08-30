#include "gamerenderglwidget.h"
#include "../utils/consts.h"
#include "./scene/gamescene.h"
using namespace parkour;

GameRenderGLWidget::GameRenderGLWidget(QWidget* parent)
    : QOpenGLWidget(parent)
    , renderTick(0)
    , lastUpdateTime(std::chrono::high_resolution_clock::now()) {
    QSurfaceFormat fmt(QSurfaceFormat::DebugContext);
    setFormat(fmt);
    currentScene = QSharedPointer<GameScene>::create(this);

	auto *timer = new QTimer(this);
    timer->setInterval(TICK_LENGTH * 1000);
	timer->setTimerType(Qt::TimerType::PreciseTimer);
	timer->moveToThread(&timerThread);

	connect(&timerThread, SIGNAL(started()), timer, SLOT(start()));
	connect(&timerThread, SIGNAL(finished()), timer, SLOT(deleteLater()));
	connect(timer, SIGNAL(timeout()), this, SLOT(tick()));

	timerThread.start();
}

GameRenderGLWidget::~GameRenderGLWidget() {
	timerThread.quit();
	timerThread.wait();
}

void GameRenderGLWidget::tick() {
    if (currentScene != nullptr) {
        currentScene->calculate();
    }
    if ((renderTick++) % TICKS_PER_FRAME == 0) {
        this->update();
    }
}

bool GameRenderGLWidget::event(QEvent* e) {
    if (currentScene != nullptr) {
        switch (e->type()) {
        case QEvent::MouseMove: // 只允许场景直接监听键盘鼠标相关事件
        case QEvent::MouseButtonPress:
        case QEvent::MouseButtonRelease:
        case QEvent::MouseButtonDblClick:
        case QEvent::KeyPress:
        case QEvent::KeyRelease:
            return currentScene->event(e);
        default:
            break;
        }
    }
    return QOpenGLWidget::event(e);
}

void GameRenderGLWidget::resizeEvent(QResizeEvent* e) {
    resizeGL(width(), height());
    QOpenGLWidget::resizeEvent(e);
}

void GameRenderGLWidget::paintFps(QPainter& p) {
    std::chrono::duration<double, std::milli> interval = (std::chrono::high_resolution_clock::now() - this->lastUpdateTime);
    p.setPen(Qt::blue);
    auto fps = qRound(1000.0 / interval.count());
    QFont font("Consolas");
    font.setPixelSize(20);
    p.setFont(font);
    p.drawText(QPoint(0, 20), QString::number(fps));
}

// todo: switch to QGraphicsView for different scenes
void GameRenderGLWidget::paintGL() {
    QPainter p(this);

    // clear screen
    p.beginNativePainting();
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_SCISSOR_TEST); // 不渲染屏幕外的像素
    glScissor(0, 0, width(), height());
    glDisable(GL_MULTISAMPLE);
    p.endNativePainting();

    if (currentScene != nullptr) {
        currentScene->repaint(p, *(this->context()));
    }

    paintFps(p);
    this->lastUpdateTime = std::chrono::high_resolution_clock::now();
}

void GameRenderGLWidget::initializeGL() {
    initializeOpenGLFunctions();
    if (currentScene != nullptr) {
        currentScene->initializeGL();
    }
}
