#include "guipause.h"
#include "model/world.h"
#include "utils/consts.h"
#include <QDebug>
#include <QLocale>
#include <QMouseEvent>
#include <QRect>

namespace parkour {
GUIPause::GUIPause() {
    widgetTexture.load(":/assets/gui/widgets.png");
}
void GUIPause::paintGUI(QPainter& p) {
    const auto deviceWidth = p.device()->width(), deviceHeight = p.device()->height();
    QRect btnSourceRect(QPoint(1, 77), QSize(198, 18));
    QRect btnHoverSourceRect(QPoint(1, 97), QSize(198, 18));

    const auto ratio = 0.4 * deviceWidth / btnSourceRect.width();

    auto font = p.font();
    font.setPointSize(11 / 854.0 * deviceWidth);
    auto smallFont = font;
    smallFont.setPointSizeF(smallFont.pointSizeF() * 0.8);

    p.setFont(font);
    p.setPen(Qt::white);

    QRect textRect(QPoint(0.3 * deviceWidth, 0.3 * deviceHeight), QSize(0.4 * deviceWidth, 0.1 * deviceHeight));
    p.drawText(textRect, "游戏菜单", Qt::AlignCenter | Qt::AlignHCenter);

    p.setFont(smallFont);
    p.setPen(Qt::gray);

    auto minutes = World::instance().getTicksFromBirth() / TICKS_PER_SEC / 60;
    auto playTime = QString("游戏时间：%0 小时 %1 分钟").arg(minutes / 60).arg(minutes % 60);
    QRect timeRect(textRect.bottomLeft(), QSize(0.4 * deviceWidth, 0.06 * deviceHeight));
    p.drawText(timeRect, playTime, Qt::AlignCenter | Qt::AlignHCenter);

    p.setFont(font);
    p.setPen(Qt::white);

    for (int i = 0; i < BUTTON_CNT; i++) {
        QPoint topLeft = (i > 0 ? buttonRects[i - 1].bottomLeft() : timeRect.bottomLeft()) + QPoint(0, 10 * ratio);
        buttonRects[i] = QRect(topLeft, QSize(0.4 * deviceWidth, 0.4 * deviceWidth * btnSourceRect.height() / btnSourceRect.width()));
        p.drawImage(buttonRects[i], widgetTexture,
            buttonRects[i].contains(mouseX, mouseY) ? btnHoverSourceRect : btnSourceRect);
    }

    p.drawText(buttonRects[0], "返回游戏", Qt::AlignCenter | Qt::AlignHCenter);
    p.drawText(buttonRects[1], "保存并退出", Qt::AlignCenter | Qt::AlignHCenter);
}
bool GUIPause::event(QEvent* evt) {
    if (evt->type() == QEvent::MouseMove) {
        QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(evt);
        mouseX = mouseEvent->x(), mouseY = mouseEvent->y();
        return true;
    } else if (evt->type() == QEvent::MouseButtonPress) {
        QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(evt);
        if (buttonRects[0].contains(mouseEvent->x(), mouseEvent->y())) {
            emit quitGUI();
        } else if (buttonRects[1].contains(mouseEvent->x(), mouseEvent->y())) {
            emit saveAndExit();
        }
        return true;
    }
    return GUIBase::event(evt);
}
}
