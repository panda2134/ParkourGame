#ifndef SAVESELECTION_H
#define SAVESELECTION_H

#include "../controller/savemanager.h"
#include <QDialog>
#include <QList>
#include <QListWidget>

namespace Ui {
class SaveSelection;
};

class SaveSelection : public QDialog {
    Q_OBJECT

    QString saveName;
    bool exists = false;

public:
    SaveSelection(QWidget* parent = nullptr);
    ~SaveSelection();
    QString getSaveName();
    bool doesExist();

private slots:
    void createButtonClicked();
    void deleteButtonClicked();
    void existingSaveClicked(QListWidgetItem* w);

private:
    Ui::SaveSelection* ui;
    QList<parkour::SaveManager::SaveMeta> meta;
    void refreshList();
};

#endif