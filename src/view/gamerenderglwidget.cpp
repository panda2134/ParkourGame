#include "gamerenderglwidget.h"

GameRenderGLWidget::GameRenderGLWidget(QWidget* parent)
    : QOpenGLWidget(parent)
    , e1(QVector2D(1, 1), QVector2D(3, 3))
    , e2(QVector2D(1, 1), QVector2D(3, 3))
    , lastUpdateTime(std::chrono::high_resolution_clock::now()) {
    this->e2.setPosition(QVector2D(70, 0));
    this->e1.setVelocity(QVector2D(3, 0));
    this->e2.setVelocity(QVector2D(-5, 0));
    this->e1.setAcceleration(QVector2D(3, 0));
    this->e2.setAcceleration(QVector2D(0, 0));
    timer.setInterval(parkour::TICK_LENGTH * 1000);
    qDebug() << 'INIT';
    connect(&timer, SIGNAL(timeout()), this, SLOT(doTickUpdate()));
    timer.start();
}

void GameRenderGLWidget::doTickUpdate() {
    this->e1.updatePosition();
    this->e2.updatePosition();
    auto face = this->e1.checkCollideWith(e2);
    qDebug() << face;
    if (face != parkour::Direction::UNKNOWN) {
        using parkour::geometry::normalizeDenominater;
        e1.collide(e2, face);
        e2.collide(e1, static_cast<parkour::Direction>(static_cast<int>(face) ^ 1));

        e1.setPosition(e1.getPosition() - e1.getVelocity() * parkour::TICK_LENGTH);
        e2.setPosition(e2.getPosition() - e2.getVelocity() * parkour::TICK_LENGTH);

        auto newVelocity = (e1.getVelocity() + e2.getVelocity()) / 2;
        e1.setVelocity(newVelocity);
        e2.setVelocity(newVelocity);
        e1.setPosition(e1.getPosition() + parkour::TICK_LENGTH * newVelocity); // 补一帧，修复共同加速之情况
        e2.setPosition(e2.getPosition() + parkour::TICK_LENGTH * newVelocity);
        //        qDebug() << e1.getPosition() << e2.getPosition() << newVelocity;
    }
    e1.update();
    e2.update();
    this->update();
}

void GameRenderGLWidget::paintGL() {
    QPainter p(this);
    p.beginNativePainting();
    auto func = this->context()->functions();
    func->glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    func->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    p.endNativePainting();
    std::chrono::duration<double, std::milli> interval = (std::chrono::high_resolution_clock::now() - this->lastUpdateTime);
    p.setPen(Qt::red);
    auto fps = qRound(1000.0 / interval.count());
    QFont font("Arial");
    font.setPixelSize(40);
    p.setFont(font);
    p.drawText(QPoint(40, 400), QString::fromStdString(std::to_string(fps)));

    float x1 = (e1.getBoundingBoxWorld().getMinX()),
          x2 = (e1.getBoundingBoxWorld().getMaxX()),
          y1 = (e1.getBoundingBoxWorld().getMinY()),
          y2 = (e1.getBoundingBoxWorld().getMaxY());
    x1 *= 10, x2 *= 10, y1 *= 10, y2 *= 10;
    //    qDebug() << "A:" << x1 << ',' << y1 << '\n';
    //    qDebug() << e1.getVelocity() << ',' << e2.getVelocity();
    p.fillRect(QRect(QPoint(x1, y1), QSize(x2 - x1, y2 - y1)), QColor(255, 120, 0));
    x1 = (e2.getBoundingBoxWorld().getMinX()),
    x2 = (e2.getBoundingBoxWorld().getMaxX()),
    y1 = (e2.getBoundingBoxWorld().getMinY()),
    y2 = (e2.getBoundingBoxWorld().getMaxY());
    //    qDebug() << "B:" << x1 << ',' << y1 << '\n';
    x1 *= 10, x2 *= 10, y1 *= 10, y2 *= 10;
    p.fillRect(QRect(QPoint(x1, y1), QSize(x2 - x1, y2 - y1)), QColor(255, 120, 0));

    this->lastUpdateTime = std::chrono::high_resolution_clock::now();
}
