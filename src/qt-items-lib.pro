include(../common.pri)

QT += core gui widgets

TARGET = qt-items
TEMPLATE = lib
VERSION = 0.0.1

DEFINES += QT_ITEMS_LIBRARY

SOURCES += QiAPI.cpp \
    utils/Range.cpp \
    utils/RangesBasic.cpp \
    utils/Layout.cpp \
    utils/LayoutBasic.cpp \
    utils/View.cpp \
    utils/ViewBasic.cpp \
    utils/ViewGrid.cpp \
    utils/ControllerMouse.cpp \
    utils/Cache.cpp \
    utils/CacheGrid.cpp \
    utils/CacheController.cpp \
    utils/Lines.cpp \
    utils/Grid.cpp \
    utils/CellsSchema.cpp \
    widgets/ItemWidget_p.cpp \
    widgets/ItemWidget.cpp \
    widgets/GridWidget.cpp \
    widgets/ListWidget.cpp

HEADERS +=  QiAPI.h \
    utils/Signal.h \
    utils/CellID.h \
    utils/Range.h \
    utils/RangesBasic.h \
    utils/Layout.h \
    utils/LayoutBasic.h \
    utils/View.h \
    utils/ViewBasic.h \
    utils/ViewGrid.h \
    utils/ControllerMouse.h \
    utils/Cache.h \
    utils/CacheGrid.h \
    utils/CacheController.h \
    utils/Lines.h \
    utils/Grid.h \
    utils/CellsSchema.h \
    widgets/ItemWidget_p.h \
    widgets/ItemWidget.h \
    widgets/GridWidget.h \
    widgets/ListWidget.h

win32 {
    TARGET_EXT = .dll
} else:unix {
    target.path = /usr/lib
    INSTALLS += target
}
