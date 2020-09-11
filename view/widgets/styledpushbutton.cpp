#include "styledpushbutton.h"
#include <QColor>
#include <QPainter>
#include <QPoint>
#include <QSize>

StyledPushButton::StyledPushButton(QWidget* parent)
    : QPushButton(parent)
    , btnSourceRect(QPoint(1, 77), QSize(198, 18))
    , btnHoverSourceRect(QPoint(1, 97), QSize(198, 18))
    , btnDisabledSourceRect(QPoint(1, 57), QSize(198, 18)) {
    this->setMouseTracking(true);
    widgetTexture.load(":/assets/gui/widgets.png");
}

void StyledPushButton::paintEvent(QPaintEvent* e) {
    Q_UNUSED(e)

    QPainter p(this);
    if (this->isEnabled()) {
        if (this->underMouse()) {
            p.drawImage(QRect(QPoint(0, 0), QSize(width(), height())), widgetTexture, btnHoverSourceRect);
        } else {
            p.drawImage(QRect(QPoint(0, 0), QSize(width(), height())), widgetTexture, btnSourceRect);
        }
        p.setPen(Qt::white);
    } else {
        p.drawImage(QRect(QPoint(0, 0), QSize(width(), height())), widgetTexture, btnDisabledSourceRect);
        p.setPen(QColor(160, 160, 160));
    }
    p.setFont(font());
    p.drawText(QRect(QPoint(0, 0), QSize(width(), height())), text(), Qt::AlignCenter | Qt::AlignHCenter);
}
