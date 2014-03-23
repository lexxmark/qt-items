include(../common.pri)

QT       += core
#QT       += script
QT       += testlib

TARGET = qi-tests

CONFIG   += console
CONFIG   -= app_bundle

INCLUDEPATH += $$ROOT_DIR/src/
LIBS += -L$$DESTDIR -lqt-items

TEMPLATE = app

HEADERS +=  test_signal.h \
    test_cell_id.h \
    test_ranges.h

SOURCES +=  main.cpp \
    test_signal.cpp \
    test_cell_id.cpp \
    test_ranges.cpp
