#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWindow>
#include <QString>
#include <QProgressDialog>
#include <QSharedPointer>
#include "./saveselection.h"
#include "./gamerenderglwidget.h"
#include "scene/gamescene.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
	Q_OBJECT

	parkour::SceneMode mode;

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
	void showProgress(int);
	void progressDone();

private slots:
	void save();

private:
    Ui::MainWindow* ui;
	QProgressDialog *progressDialog;
	GameRenderGLWidget *gameWidget;
	SaveSelection *saveSelectionWidget;
	void selectSave(parkour::SceneMode mode);
	void initProgress(QString title);
};
#endif // MAINWINDOW_H
