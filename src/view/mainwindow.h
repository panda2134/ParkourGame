#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "../model/testentity.h"
#include "../utils/consts.h"
#include <QDebug>
#include <QMainWindow>
#include <QPainter>
#include <QTimer>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

    parkour::TestEntity e1, e2;
    QTimer timer;
public slots:
    void doTickUpdate();

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow* ui;

protected:
    void paintEvent(QPaintEvent*);
};
#endif // MAINWINDOW_H
