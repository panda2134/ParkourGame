#ifndef STYLEDPUSHBUTTON_H
#define STYLEDPUSHBUTTON_H

#include <QPushButton>
#include <QImage>
#include <QRect>

class StyledPushButton : public QPushButton
{
	Q_OBJECT

	QImage widgetTexture;
	QRect btnSourceRect, btnHoverSourceRect, btnDisabledSourceRect;

public:
	StyledPushButton(QWidget *parent = nullptr);


	void paintEvent(QPaintEvent *e) override;
};

#endif