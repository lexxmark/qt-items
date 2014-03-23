include(../common.pri)

QT += core gui widgets

TARGET = qt-items
TEMPLATE = lib
VERSION = 0.0.1

DEFINES += QI_LIBRARY

SOURCES += QiAPI.cpp \
    utils/Range.cpp \
    utils/RangesBasic.cpp \
    utils/Layout.cpp \
    utils/View.cpp \
    widgets/GridWidget.cpp \
    widgets/ItemWidget.cpp \
    widgets/ListWidget.cpp

HEADERS +=  QiAPI.h \
    utils/Signal.h \
    utils/CellID.h \
    utils/Range.h \
    utils/RangesBasic.h \
    utils/Layout.h \
    utils/View.h \
    widgets/GridWidget.h \
    widgets/ItemWidget.h \
    widgets/ListWidget.h

win32 {
    TARGET_EXT = .dll
} else:unix {
    target.path = /usr/lib
    INSTALLS += target
}
