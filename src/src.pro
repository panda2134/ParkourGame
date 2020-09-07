QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17
QMAKE_CXXFLAGS += /utf-8

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    controller/playercontroller.cpp \
    controller/worldcontroller.cpp \
    main.cpp \
    model/block.cpp \
    model/blockdelegate.cpp \
    model/blockdirt.cpp \
    model/blockgrass.cpp \
    model/blockspike.cpp \
    model/blockspring.cpp \
    model/blocktnt.cpp \
    model/blocksaw.cpp \
    model/boundingbox.cpp \
    model/entity.cpp \
    model/entityblaze.cpp \
    model/entityfireball.cpp \
    model/entityplayer.cpp \
    model/entitytnt.cpp \
    model/icollidable.cpp \
    model/registry.cpp \
    model/world.cpp \
    model/entityplayerlike.cpp \
    utils/direction.cpp \
    utils/geometryhelper.cpp \
    utils/glhelper.cpp \
    view/gamerenderglwidget.cpp \
    view/mainwindow.cpp \
    view/scene/gamescene.cpp \
    view/scene/iscene.cpp \
    model/blockspike.cpp \
    model/blockbedrock.cpp \
    model/blockstonebrick.cpp \
    model/entitymovingbrick.cpp


HEADERS += \
    controller/playercontroller.h \
    controller/worldcontroller.h \
    model/blockspike.h \
    model/blockbedrock.h \
    model/blockspring.h \
    model/blockstonebrick.h\
    model/entityblaze.h \
    model/entityfireball.h \
    model/entitymovingbrick.h\
    model/block.h \
    model/blockdelegate.h \
    model/blockdirt.h \
    model/blockgrass.h \
    model/blocktnt.h \
    model/blocksaw.h \
    model/boundingbox.h \
    model/entity.h \
    model/entityplayer.h \
    model/entityplayerlike.h \
    model/entitytnt.h \
    model/icollidable.h \
    model/registry.h \
    model/world.h \
    utils/askeyvaluerange.h \
    utils/consts.h \
    utils/direction.h \
    utils/geometryhelper.h \
    utils/glhelper.h \
    utils/singleton.h \
    view/gamerenderglwidget.h \
    view/mainwindow.h \
    view/scene/gamescene.h \
    view/scene/iscene.h 

FORMS += \
    view/mainwindow.ui


release: DESTDIR = ../bin/release
debug:   DESTDIR = ../bin/debug

OBJECTS_DIR = $$DESTDIR/.obj
MOC_DIR = $$DESTDIR/.moc
RCC_DIR = $$DESTDIR/.qrc
UI_DIR = $$DESTDIR/.ui

RESOURCES += \
    res.qrc
