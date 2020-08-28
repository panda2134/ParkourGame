#ifndef ISCENE_H
#define ISCENE_H

#include <QObject>
#include <QOpenGLContext>
#include <QPainter>

namespace parkour {

class IScene : public QObject {
    Q_OBJECT
public:
    explicit IScene(QObject* parent = nullptr);

    virtual void calculate() {};
    virtual void repaint(QPainter&, QOpenGLContext&) = 0;

signals:
    void switchToScene(QString);
};
}

#endif // ISCENE_H
