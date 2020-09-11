#include "inventorygui.h"
#include "controller/worldcontroller.h"
#include <QEvent>
#include <QMouseEvent>
#include <QPoint>

namespace parkour {
QRect InventoryGUI::slotOnScreen(const QRect& rect) {
    return QRect(rect.topLeft() * getRatio() + screenTopLeft, rect.size() * getRatio());
}
double InventoryGUI::getRatio() {
    return 0.35 * deviceWidth / inventoryTexture.width();
}
InventoryGUI::InventoryGUI()
    : mouseX(0)
    , mouseY(0)
    , dragIndex(-1) {
    inventoryTexture.load(":/assets/gui/inventory.png");
    const QPoint backpackStart(9, 8), hotbarStart(9, 66);
    const int gap = 2, slotWidth = 16;
    for (int i = 0; i < 27; i++) {
        const int row = i / 9, col = i % 9;
        inventorySlotRects[i] = QRect(backpackStart + QPoint(col * (gap + slotWidth), row * (gap + slotWidth)),
            QSize(slotWidth, slotWidth));
    }
    for (int i = 27; i < 36; i++) {
        const int row = 3, col = i - 27;
        inventorySlotRects[i] = QRect(hotbarStart + QPoint(col * (gap + slotWidth), 0), QSize(slotWidth, slotWidth));
    }
}
InventoryGUI::~InventoryGUI() {
    if (dragIndex != -1) {
        auto player = WorldController::instance().getPlayerController()->getPlayer();
        if (player != nullptr) {
            player->getInventory()[dragIndex] = draggingItem;
        }
    }
}
void InventoryGUI::paintGUI(QPainter& p) {
    if (inventoryTexture.isNull()) {
        return;
    }
    deviceWidth = p.device()->width();
    deviceHeight = p.device()->height();

    // draw inventory background
    screenTopLeft = QPoint(0.5 * (deviceWidth - getRatio() * inventoryTexture.width()),
        0.5 * (deviceHeight - getRatio() * inventoryTexture.height()));
    screenTextureSize = inventoryTexture.size() * getRatio();
    p.drawImage(QRect(screenTopLeft, screenTextureSize), inventoryTexture);

    // draw inventory content
    auto player = WorldController::instance().getPlayerController()->getPlayer();
    if (player != nullptr) {
        auto* inv = player->getInventory();
        for (int i = 0; i < PLAYER_INVENTORY_SLOT_COUNT; i++) {
            if (inv[i] == nullptr) {
                continue;
            }
            p.drawImage(slotOnScreen(inventorySlotRects[i]), inv[i]->getIcon());
        }
    }

    // draw current slot
    for (auto& rect : inventorySlotRects) {
        if (slotOnScreen(rect).contains(mouseX, mouseY)) {
            p.fillRect(slotOnScreen(rect), QBrush(QColor(255, 255, 255, 100)));
            break;
        }
    }
    // TODO: draw drag 'n drop
    if (draggingItem != nullptr) {
        const auto& icon = draggingItem->getIcon();
        p.drawImage(QRect((QVector2D(mouseX, mouseY) - dragOffset).toPoint(), icon.size() * getRatio()), icon);
    }
}
bool InventoryGUI::event(QEvent* evt) {
    if (evt->type() == QEvent::MouseMove) {
        QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(evt);
        mouseX = mouseEvent->x(), mouseY = mouseEvent->y();
        return true;
    } else if (evt->type() == QEvent::MouseButtonPress) {
        QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(evt);
        auto player = WorldController::instance().getPlayerController()->getPlayer();
        if (player != nullptr) {
            auto* inv = player->getInventory();

            for (int index = 0; index < PLAYER_INVENTORY_SLOT_COUNT; index++) {
                const auto& screenSlot = slotOnScreen(inventorySlotRects[index]);
                if (screenSlot.contains(mouseX, mouseY)) {
                    if (mouseEvent->modifiers().testFlag(Qt::KeyboardModifier::ShiftModifier)) {
                        if (index < 27) {
                            for (int j = 27; j < PLAYER_INVENTORY_SLOT_COUNT; j++) {
                                if (inv[j] == nullptr) {
                                    qSwap(inv[index], inv[j]);
                                    return true;
                                }
                            }
                        } else {
                            for (int j = 0; j < 27; j++) {
                                if (inv[j] == nullptr) {
                                    qSwap(inv[index], inv[j]);
                                    return true;
                                }
                            }
                        }
                    } else {
                        qSwap(draggingItem, inv[index]);
                        if (draggingItem != nullptr) {
                            dragIndex = index;
                            dragOffset = QVector2D(mouseX, mouseY) - QVector2D(screenSlot.topLeft()) - QVector2D(2, 2) * getRatio();
                        } else {
                            dragIndex = -1;
                        }
                        return true;
                    }
                    break;
                }
            }
        }
    }
    return GUIBase::event(evt);
}
}
