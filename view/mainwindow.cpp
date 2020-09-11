#include "mainwindow.h"
#include "controller/savemanager.h"
#include "controller/worldcontroller.h"
#include "model/world.h"
#include "ui_mainwindow.h"
#include "utils/worldioworker.h"
#include "view/scene/gamesound.h"
#include <QDebug>
#include <functional>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , saveSelectionWidget(nullptr)
    , gameWidget(nullptr)
    , progressDialog(nullptr)
    , mode(parkour::SceneMode::GAMING) {
    ui->setupUi(this);
    connect(ui->playButton, &StyledPushButton::clicked, [this]() { this->selectSave(parkour::SceneMode::GAMING); });
    connect(ui->editButton, &StyledPushButton::clicked, [this]() { this->selectSave(parkour::SceneMode::MAPEDIT); });
    auto& ioWorker = parkour::WorldIOWorker::instance();
    connect(&ioWorker, &parkour::WorldIOWorker::progress, this, &MainWindow::showProgress);
    connect(&ioWorker, &parkour::WorldIOWorker::done, this, &MainWindow::progressDone);
}

void MainWindow::showProgress(int val) {
    if (this->progressDialog == nullptr) {
        return;
    }
    this->progressDialog->setValue(val);
}

void MainWindow::save() {
    this->gameWidget = nullptr;
    auto saveName = this->saveSelectionWidget->getSaveName();
    initProgress("保存中...");
    parkour::WorldIOWorker::executeWork([saveName]() {
        parkour::SaveManager::instance().writeToSave(saveName);
    });
}

void MainWindow::progressDone() {
    this->progressDialog->close();
    delete this->progressDialog;
    this->progressDialog = nullptr;

    if (parkour::World::instance().isReady()) { // 若为加载世界则打开游戏窗口
        this->gameWidget = new GameRenderGLWidget();
        connect(this->gameWidget, &GameRenderGLWidget::destroyed, this, &MainWindow::save);
        this->gameWidget->gameScene->mode = mode;
        this->gameWidget->show();
        this->hide();
    } else {
        this->show();
    }
}

void MainWindow::selectSave(parkour::SceneMode mode) {
    this->saveSelectionWidget = new SaveSelection(this);
    this->saveSelectionWidget->setModal(true);
    bool res = this->saveSelectionWidget->exec();

    if (res) {
        this->mode = mode;
        const auto& saveName = this->saveSelectionWidget->getSaveName();
        initProgress("加载中...");
        if (this->saveSelectionWidget->doesExist()) {
            parkour::WorldIOWorker::executeWork([saveName]() {
                parkour::SaveManager::instance().loadFromSave(saveName);
            });
        } else {
            parkour::WorldIOWorker::executeWork([]() {
                parkour::WorldController::instance().loadInitialWorld();
            });
        }
    }
}

void MainWindow::initProgress(QString title) {
    progressDialog = new QProgressDialog(this);
    progressDialog->setModal(true);
    progressDialog->setCancelButton(nullptr);
    progressDialog->setLabelText(title);
    progressDialog->show();
}

MainWindow::~MainWindow() {
    delete ui;
}
