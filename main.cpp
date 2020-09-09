#include "view/mainwindow.h"
#include "view/scene/gamesound.h"
#include "model/registry.h"
#include "utils/worldioworker.h"

#include <QApplication>
#include <QPixmap>
#include <QSplashScreen>

void processAppEvents(QApplication& app) {
    const int EVENT_COUNT_PER_CALL = 5000;
    for (int i = 0; i < EVENT_COUNT_PER_CALL; i++) {
        app.processEvents();
    }
}

int main(int argc, char *argv[])
{
    using namespace parkour;

    QApplication a(argc, argv);

    QPixmap pixmap(":/assets/gui/splash.png");
    QSplashScreen splash(pixmap);
    splash.show();

    processAppEvents(a);
    splash.showMessage("Loading blocks...", Qt::AlignBottom);
    parkour::registry::BlockRegistry::instance();

    processAppEvents(a);
    splash.showMessage("Loading entities...", Qt::AlignBottom);
    parkour::registry::EntityRegistry::instance();

    processAppEvents(a);
    splash.showMessage("Loading items...", Qt::AlignBottom);
    parkour::registry::ItemRegistry::instance();

    processAppEvents(a);
    splash.showMessage("Loading sounds...", Qt::AlignBottom);
    parkour::GameSound::instance();

    processAppEvents(a);
    splash.showMessage("Loading workers...", Qt::AlignBottom);
    parkour::WorldIOWorker::instance();

    processAppEvents(a);
    splash.showMessage("Loading Main Screen...", Qt::AlignBottom);

    MainWindow w;
    w.show();
    processAppEvents(a);
    splash.finish(&w);

    return a.exec();
}
