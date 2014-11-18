include(../common.pri)

QT       += core widgets
#QT      += script
QT       += testlib

TARGET = qi-tests

CONFIG   += console
CONFIG   -= app_bundle

INCLUDEPATH += $$ROOT_DIR/src/
LIBS += -L$$DESTDIR -lqt-items

TEMPLATE = app

HEADERS +=  SignalSpy.h\
    test_signal.h \
    test_item_id.h \
    test_ranges.h \
    test_lines.h \
    test_grid.h

SOURCES +=  main.cpp \
    test_signal.cpp \
    test_item_id.cpp \
    test_ranges.cpp \
    test_lines.cpp \
    test_grid.cpp
