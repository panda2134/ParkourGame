#include "mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::doTickUpdate() {
    this->e1.updatePosition();
    this->e2.updatePosition();
    auto face = this->e1.checkCollideWith(e2);
    qDebug() << face;
    if (face != parkour::Direction::UNKNOWN) {
        e1.collide(e2, face);
        e2.collide(e1, static_cast<parkour::Direction>(static_cast<int>(face) ^ 1));
        e1.setPosition(e1.getPosition() - e1.getVelocity() * parkour::TICK_LENGTH);
        e2.setPosition(e2.getPosition() - e2.getVelocity() * parkour::TICK_LENGTH);
        auto newVelocity = (e1.getVelocity() + e2.getVelocity()) / 2;
        e1.setVelocity(newVelocity);
        e2.setVelocity(newVelocity);
    }
    e1.update();
    e2.update();
    this->update();
}

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow) {
    ui->setupUi(this);
    this->e1.setPosition(QVector2D(30, 40));
    this->e1.setVelocity(QVector2D(10, 10));
    timer.setInterval(parkour::TICK_LENGTH * 1000);
    connect(&timer, SIGNAL(timeout()), this, SLOT(doTickUpdate()));
    timer.start();
}

void MainWindow::paintEvent(QPaintEvent* evt) {
    QPainter p(this);
    float x1 = e1.getBoundingBoxWorld().getMinX(),
          x2 = e1.getBoundingBoxWorld().getMaxX(),
          y1 = e1.getBoundingBoxWorld().getMinY(),
          y2 = e1.getBoundingBoxWorld().getMaxY();
    qDebug() << "A:" << x1 << ',' << y1 << '\n';
    qDebug() << e1.getVelocity() << ',' << e2.getVelocity();
    p.drawRect(QRect(QPoint(x1, y1), QSize(x2 - x1, y2 - y1)));
    x1 = e2.getBoundingBoxWorld().getMinX(),
    x2 = e2.getBoundingBoxWorld().getMaxX(),
    y1 = e2.getBoundingBoxWorld().getMinY(),
    y2 = e2.getBoundingBoxWorld().getMaxY();
    qDebug() << "B:" << x1 << ',' << y1 << '\n';
    p.drawRect(QRect(QPoint(x1, y1), QSize(x2 - x1, y2 - y1)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

