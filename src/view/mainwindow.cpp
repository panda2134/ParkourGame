#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "../model/world.h"
#include "../controller/savemanager.h"
#include "../controller/worldcontroller.h"
#include "../controller/worldioworker.h"
#include <QDebug>
#include <functional>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , saveSelectionWidget(nullptr)
    , gameWidget(nullptr)
    , progressDialog(nullptr) {
    ui->setupUi(this);
	connect(ui->playButton, &StyledPushButton::clicked, [this]() { this->selectSave(parkour::SceneMode::GAMING); });
	connect(ui->editButton, &StyledPushButton::clicked, [this]() { this->selectSave(parkour::SceneMode::MAPEDIT); });
	auto &ioWorker = parkour::WorldIOWorker::instance();
	connect(&ioWorker, &parkour::WorldIOWorker::progress, this, &MainWindow::showProgress);
	connect(&ioWorker, &parkour::WorldIOWorker::done, this, &MainWindow::progressDone);
	qDebug() << "inited";
}

void MainWindow::showProgress(int val) {
	this->progressDialog->setValue(val);
}

void MainWindow::progressDone() {
	this->progressDialog->close();
	if (parkour::World::instance().isReady()) { // 若为加载世界则打开游戏窗口
		if (gameWidget != nullptr) {
			return;
		}
		this->gameWidget = new GameRenderGLWidget();
		connect(this->gameWidget, &GameRenderGLWidget::destroyed, [this]() {
			this->gameWidget = nullptr;
			auto saveName = this->saveSelectionWidget->getSaveName();
			initProgress("保存中...");
			parkour::WorldIOWorker::executeWork([saveName]() {
				parkour::SaveManager::instance().writeToSave(saveName);
			});
		});
		this->gameWidget->gameScene->mode = mode;
		this->gameWidget->show();
	} else {
		this->ui->playButton->show();
		this->ui->editButton->show();
	}
}

void MainWindow::selectSave(parkour::SceneMode mode) {
	this->saveSelectionWidget = new SaveSelection(this);
	this->hide();
	bool res = this->saveSelectionWidget->exec();
	this->show();

	if (res) {
		this->mode = mode;
		const auto &saveName = this->saveSelectionWidget->getSaveName();
		initProgress("加载中...");
		if (this->saveSelectionWidget->doesExist()) {
			// load existing
			qDebug() << "Exists" << saveName;
			parkour::WorldIOWorker::executeWork([saveName]() {
				parkour::SaveManager::instance().loadFromSave(saveName);
			});
		} else {
			// load new world
			qDebug() << "New" << saveName;
			parkour::WorldIOWorker::executeWork([]() {
				parkour::WorldController::instance().loadInitialWorld();
			});
		}
	}
}

void MainWindow::initProgress(QString title) {
	if (progressDialog != nullptr) {
		delete progressDialog;
	}
	this->ui->playButton->hide();
	this->ui->editButton->hide();
	progressDialog = new QProgressDialog(this);
	progressDialog->setCancelButton(nullptr);
	progressDialog->setLabelText(title);
	progressDialog->show();
}

MainWindow::~MainWindow() {
    delete ui;
}
