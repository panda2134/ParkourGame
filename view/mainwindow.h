#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "./gamerenderglwidget.h"
#include "./saveselection.h"
#include "scene/gamescene.h"
#include <QMainWindow>
#include <QProgressDialog>
#include <QSharedPointer>
#include <QString>
#include <QWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

    parkour::SceneMode mode;

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

public slots:
    void showProgress(int);
    void progressDone();

private slots:
    void save();

private:
    Ui::MainWindow* ui;
    QProgressDialog* progressDialog;
    GameRenderGLWidget* gameWidget;
    SaveSelection* saveSelectionWidget;
    void selectSave(parkour::SceneMode mode);
    void initProgress(QString title);
};
#endif // MAINWINDOW_H
