#ifndef SAVEITEM_H
#define SAVEITEM_H

#include <QWidget>
#include "ui_saveitem.h"
#include "../controller/savemanager.h"

class SaveItem : public QWidget
{
	Q_OBJECT

	typedef parkour::SaveManager::SaveMeta SaveMeta;
	SaveMeta meta;

public:
	void setMeta(SaveMeta meta);
	SaveMeta getMeta() const;

	SaveItem(QWidget *parent = nullptr);
	~SaveItem();

private:
	Ui::SaveItem ui;
};

#endif