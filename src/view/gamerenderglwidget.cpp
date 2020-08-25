#include "gamerenderglwidget.h"

GameRenderGLWidget::GameRenderGLWidget(QWidget* parent)
    : QOpenGLWidget(parent)
    , e1(QVector2D(50, 50))
    , e2(QVector2D(30, 30)) {
    this->e2.setPosition(QVector2D(750, 0));
    this->e1.setVelocity(QVector2D(30, 0));
    this->e2.setVelocity(QVector2D(-50, 0));
    this->e1.setAcceleration(QVector2D(0, 9.854));
    this->e2.setAcceleration(QVector2D(0, 9.854));
    timer.setInterval(parkour::TICK_LENGTH * 1000);
    qDebug() << 'INIT';
    connect(&timer, SIGNAL(timeout()), this, SLOT(doTickUpdate()));
    timer.start();
}

void GameRenderGLWidget::doTickUpdate() {
    this->e1.updatePosition();
    this->e2.updatePosition();
    auto face = this->e1.checkCollideWith(e2);
    //    qDebug() << face;
    if (face != parkour::Direction::UNKNOWN) {
        using parkour::geometry::normalizeDenominater;
        e1.collide(e2, face);
        e2.collide(e1, static_cast<parkour::Direction>(static_cast<int>(face) ^ 1));

        e1.setPosition(e1.getPosition() - e1.getVelocity() * parkour::TICK_LENGTH);
        e2.setPosition(e2.getPosition() - e2.getVelocity() * parkour::TICK_LENGTH);
        auto newVelocity = (e1.getVelocity() + e2.getVelocity()) / 2;
        qDebug() << e1.getVelocity() << e2.getVelocity() << newVelocity;
        e1.setVelocity(newVelocity);
        e2.setVelocity(newVelocity);
    }
    e1.update();
    e2.update();
    this->update();
}

void GameRenderGLWidget::paintEvent(QPaintEvent* evt) {
    Q_UNUSED(evt)
    QPainter p(this);
    p.fillRect(QRect(0, 0, 854, 480), QColor(255, 255, 255));
    float x1 = qRound(e1.getBoundingBoxWorld().getMinX()),
          x2 = qRound(e1.getBoundingBoxWorld().getMaxX()),
          y1 = qRound(e1.getBoundingBoxWorld().getMinY()),
          y2 = qRound(e1.getBoundingBoxWorld().getMaxY());
    //    qDebug() << "A:" << x1 << ',' << y1 << '\n';
    //    qDebug() << e1.getVelocity() << ',' << e2.getVelocity();
    p.drawRect(QRect(QPoint(x1, y1), QSize(x2 - x1, y2 - y1)));
    x1 = qRound(e2.getBoundingBoxWorld().getMinX()),
    x2 = qRound(e2.getBoundingBoxWorld().getMaxX()),
    y1 = qRound(e2.getBoundingBoxWorld().getMinY()),
    y2 = qRound(e2.getBoundingBoxWorld().getMaxY());
    //    qDebug() << "B:" << x1 << ',' << y1 << '\n';
    p.drawRect(QRect(QPoint(x1, y1), QSize(x2 - x1, y2 - y1)));
}
