#include "view/mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QSurfaceFormat fmt;
    fmt.setSwapBehavior(QSurfaceFormat::TripleBuffer);
    fmt.setSwapInterval(1);
    QSurfaceFormat::setDefaultFormat(fmt);
    MainWindow w;
    w.show();
    return a.exec();
}
