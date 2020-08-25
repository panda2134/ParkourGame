#ifndef GAMERENDERGLWIDGET_H
#define GAMERENDERGLWIDGET_H

#include "../model/testentity.h"
#include "../utils/consts.h"
#include "../utils/direction.h"
#include <QDebug>
#include <QObject>
#include <QOpenGLWidget>
#include <QPaintEvent>
#include <QPainter>
#include <QRegion>
#include <QTime>
#include <QTimer>
#include <QVector2D>
#include <QWindow>

class GameRenderGLWidget : public QOpenGLWidget {
    Q_OBJECT
public:
    GameRenderGLWidget(QWidget* parent = nullptr);

    parkour::TestEntity e1, e2;
    QTimer timer;
public slots:
    void doTickUpdate();

protected:
    void paintEvent(QPaintEvent*);
};

#endif // GAMERENDERGLWIDGET_H
