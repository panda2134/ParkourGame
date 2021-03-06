TEMPLATE = app
TARGET = MineParkour
DESTDIR = ./bin/release
CONFIG += release c++17
DEFINES += _WINDOWS _UNICODE _ENABLE_EXTENDED_ALIGNED_STORAGE WIN64 QT_DEPRECATED_WARNINGS
LIBS += -lshell32
DEPENDPATH += .
MOC_DIR += .
OBJECTS_DIR += release
UI_DIR += .
RCC_DIR += .
include(MineParkour.pri)

QT += core gui widgets multimedia
win32 {
	RC_FILE = MineParkour.rc
}

ambient.commands = $$QMAKE_COPY_DIR $$shell_quote($$shell_path($$PWD/assets/sounds/ambient)) $$shell_quote($$shell_path($$OUT_PWD/$$DESTDIR/ambient))
first.depends = $(first) ambient
export(first.depends)
export(ambient.commands)
QMAKE_EXTRA_TARGETS += first ambient

msvc* {
	QMAKE_CXXFLAGS += /utf-8 /F 16777216 /std:c++17
}

!msvc* {
	QMAKE_CXXFLAGS += -std=c++17
}

isEmpty(TARGET_EXT) {
    win32 { TARGET_CUSTOM_EXT = .exe }
    macx { TARGET_CUSTOM_EXT = .app }
} else {
    TARGET_CUSTOM_EXT = $${TARGET_EXT}
}

win32 { DEPLOY_COMMAND = $$shell_quote($$shell_path($$[QT_INSTALL_BINS]\windeployqt)) }
macx { DEPLOY_COMMAND = $$shell_quote($$shell_path($$[QT_INSTALL_BINS]/macdeployqt)) }

DEPLOY_TARGET = $$shell_quote($$shell_path($$OUT_PWD/$$DESTDIR/$$TARGET$$TARGET_CUSTOM_EXT))

QMAKE_POST_LINK = $$DEPLOY_COMMAND $$DEPLOY_TARGET
