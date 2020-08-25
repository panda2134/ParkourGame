QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

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
    main.cpp \
    model/boundingbox.cpp \
    model/entity.cpp \
    model/icollidable.cpp \
    model/testentity.cpp \
    model/world.cpp \
    utils/geometryhelper.cpp \
    view/gamerenderglwidget.cpp \
    view/mainwindow.cpp

HEADERS += \
    model/boundingbox.h \
    model/entity.h \
    model/icollidable.h \
    model/testentity.h \
    model/world.h \
    utils/consts.h \
    utils/direction.h \
    utils/geometryhelper.h \
    utils/singleton.h \
    view/gamerenderglwidget.h \
    view/mainwindow.h

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
