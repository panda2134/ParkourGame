#ifndef SAVEITEM_H
#define SAVEITEM_H

#include "../controller/savemanager.h"
#include "ui_saveitem.h"
#include <QWidget>

class SaveItem : public QWidget {
    Q_OBJECT

    typedef parkour::SaveManager::SaveMeta SaveMeta;
    SaveMeta meta;

public:
    void setMeta(SaveMeta meta);
    SaveMeta getMeta() const;

    SaveItem(QWidget* parent = nullptr);
    ~SaveItem();

private:
    Ui::SaveItem ui;
};

#endif