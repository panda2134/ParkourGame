# ----------------------------------------------------
# This file is generated by the Qt Visual Studio Tools.
# ------------------------------------------------------

TEMPLATE = app
TARGET = MineParkour
DESTDIR = ../bin/debug
CONFIG += debug
DEFINES += _WINDOWS _UNICODE _ENABLE_EXTENDED_ALIGNED_STORAGE WIN64 QT_DEPRECATED_WARNINGS
LIBS += -L"../../../../../../openssl/lib" \
    -L"../../../../../../Utils/my_sql/mysql-5.7.25-winx64/lib" \
    -L"../../../../../../Utils/postgresql/pgsql/lib" \
    -lshell32
DEPENDPATH += .
MOC_DIR += .
OBJECTS_DIR += debug
UI_DIR += .
RCC_DIR += .
include(MineParkour.pri)
