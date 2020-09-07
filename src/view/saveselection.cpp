﻿#include "saveselection.h"
#include "saveitem.h"
#include "ui_saveselection.h" 
#include "styledpushbutton.h"
#include <QDebug>
#include <QMessageBox>
#include <QInputDialog>

SaveSelection::SaveSelection(QWidget * parent) : QDialog(parent) {
	ui = new Ui::SaveSelection();
	ui->setupUi(this);
	meta = parkour::SaveManager::instance().listSaves();

	for (const auto &x : meta) {
		qDebug() << x.saveName;
		auto *w = new SaveItem(this);
		w->setMeta(x);
		auto *item = new QListWidgetItem(ui->listWidget);
		item->setSizeHint(w->sizeHint());
		ui->listWidget->setItemWidget(item, w);
	}

	connect(ui->createButton, &StyledPushButton::clicked, this, &SaveSelection::createButtonClicked);
	connect(ui->listWidget, &QListWidget::itemDoubleClicked, this, &SaveSelection::existingSaveClicked);
}

void SaveSelection::createButtonClicked() {
	bool ok = false;
	QString saveName = QInputDialog::getText(this, "新存档", "存档名称：", QLineEdit::Normal, "", &ok);
	if (!ok || saveName == "") {
		QMessageBox::critical(this, "创建失败", "存档名称为空");
		return;
	}
	for (const auto &x : meta) {
		if (x.saveName == saveName) {
			QMessageBox::critical(this, "创建失败", "存档名称已使用");
			return;
		}
	}
	if (!parkour::SaveManager::instance().createSaveFolder(saveName)) {
		QMessageBox::critical(this, "创建失败", "存档名称不合法");
	}
	this->saveName = saveName;
	this->exists = false;
	this->accept();
}

SaveSelection::~SaveSelection() {
	delete ui;
}

QString SaveSelection::getSaveName() {
	return saveName;
}

bool SaveSelection::doesExist() {
	return exists;
}

void SaveSelection::existingSaveClicked(QListWidgetItem * w) {
	auto *saveItem = static_cast<SaveItem*>(ui->listWidget->itemWidget(w));
	this->saveName = saveItem->getMeta().saveName;
	this->exists = true;
	this->accept();
}