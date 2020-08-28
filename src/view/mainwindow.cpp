#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow) {
    ui->setupUi(this);
    ui->openGLWidget->setFocus(); // 设置键盘焦点以处理所有事件
}

MainWindow::~MainWindow() {
    delete ui;
}
