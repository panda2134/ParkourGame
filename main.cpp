#include "view/mainwindow.h"
#include "view/scene/gamesound.h"
#include "model/registry.h"
#include "utils/worldioworker.h"

#include <QApplication>
#include <QPixmap>
#include <QSplashScreen>

int main(int argc, char *argv[])
{
    using namespace parkour;

    QApplication a(argc, argv);

    QPixmap pixmap(":/assets/gui/splash.png");
    QSplashScreen splash(pixmap);
    splash.show();

    a.processEvents();
    splash.showMessage("Loading blocks...", Qt::AlignBottom);
    parkour::registry::BlockRegistry::instance();

    a.processEvents();
    splash.showMessage("Loading entities...", Qt::AlignBottom);
    parkour::registry::EntityRegistry::instance();

    a.processEvents();
    splash.showMessage("Loading items...", Qt::AlignBottom);
    parkour::registry::ItemRegistry::instance();

    a.processEvents();
    splash.showMessage("Loading sounds...", Qt::AlignBottom);
    parkour::GameSound::instance();

    a.processEvents();
    splash.showMessage("Loading workers...", Qt::AlignBottom);
    parkour::WorldIOWorker::instance();

    splash.showMessage("Loading Main Screen...", Qt::AlignBottom);
    for (int i = 0; i < 100000; i++) {
        a.processEvents();
    }

    MainWindow w;
    w.show();

    splash.finish(&w);

    return a.exec();
}
