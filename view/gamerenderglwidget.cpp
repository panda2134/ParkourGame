#include "gamerenderglwidget.h"
#include "../controller/savemanager.h"
#include "../utils/consts.h"
#include "../model/world.h"
#include "./scene/gamescene.h"
#include <QApplication>
using namespace parkour;

GameRenderGLWidget::GameRenderGLWidget(QWidget* parent)
    : QOpenGLWidget(parent)
    , renderTick(0)
    , timerThread(new QThread(this)){
	this->setFocus();
	this->setMouseTracking(true);
	this->setAttribute(Qt::WA_DeleteOnClose);
	this->setAttribute(Qt::WA_QuitOnClose, false);
	this->setMinimumWidth(854);
	this->setMinimumHeight(480);
    gameScene = QSharedPointer<GameScene>::create(this);

	connect(gameScene.data(), &GameScene::saveAndExit, this, &GameRenderGLWidget::close);

    auto* timer = new QTimer();
    timer->setInterval(TICK_LENGTH * 1000);
    timer->setTimerType(Qt::TimerType::PreciseTimer);
    timer->moveToThread(timerThread);

    connect(timerThread, SIGNAL(started()), timer, SLOT(start()));
    connect(timerThread, SIGNAL(finished()), timer, SLOT(deleteLater()));
    connect(timer, SIGNAL(timeout()), this, SLOT(tick()));

    timerThread->start();
}

GameRenderGLWidget::~GameRenderGLWidget() {
	timerThread->quit();
	timerThread->wait();
	delete timerThread;
}

void GameRenderGLWidget::tick() {
	if (!parkour::World::instance().isReady()) {
		return;
	}
    if (gameScene != nullptr) {
        gameScene->calculate();
    }
    if ((renderTick++) % TICKS_PER_FRAME == 0) {
        this->update();
    }
}

bool GameRenderGLWidget::event(QEvent* e) {
    if (gameScene != nullptr) {
        switch (e->type()) {
        case QEvent::MouseMove: // 只允许场景直接监听键盘鼠标相关事件
        case QEvent::MouseButtonPress:
        case QEvent::MouseButtonRelease:
        case QEvent::KeyPress:
        case QEvent::KeyRelease:
		case QEvent::Wheel:
		case QEvent::DragMove:
		case QEvent::DragEnter:
		case QEvent::DragLeave:
			return QApplication::sendEvent(gameScene.data(), e);
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

void GameRenderGLWidget::paintGL() {
    QPainter p(this);

    // clear screen
    p.beginNativePainting();
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_SCISSOR_TEST); // 不渲染屏幕外的像素
    glScissor(0, 0, width(), height());
    p.endNativePainting();

    if (gameScene != nullptr) {
        gameScene->repaint(p, *(this->context()));
    }
}

void GameRenderGLWidget::initializeGL() {
    initializeOpenGLFunctions();
    if (gameScene != nullptr) {
        gameScene->initializeGL();
    }
}

void GameRenderGLWidget::closeEvent(QCloseEvent *) {
	gameScene->setQuitting();
	paintGL();
	SaveManager::instance().setLastScreenshot(grabFramebuffer());
}
