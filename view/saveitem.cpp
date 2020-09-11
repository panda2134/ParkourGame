#include "saveitem.h"
#include <QPixmap>

void SaveItem::setMeta(SaveMeta meta) {
    this->meta = meta;
    // paint
    auto im = QPixmap::fromImage(meta.saveImage);
    ui.imgLabel->setPixmap(im.scaled(ui.imgLabel->size()));
    ui.saveNameLabel->setText(meta.saveName);
    ui.lastModifiedLabel->setText(meta.lastModifiedTime.toString());
}

parkour::SaveManager::SaveMeta SaveItem::getMeta() const {
    return meta;
}

SaveItem::SaveItem(QWidget* parent)
    : QWidget(parent) {
    ui.setupUi(this);
}

SaveItem::~SaveItem() {
}