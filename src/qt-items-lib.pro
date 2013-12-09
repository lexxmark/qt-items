include(../common.pri)

TARGET = qt-items
TEMPLATE = lib
VERSION = 0.0.1

DEFINES += QI_LIBRARY

SOURCES += QiAPI.cpp

HEADERS +=  QiAPI.h \
            utils/QiSignal.h \
            utils/QiCellID.h

win32 {
    TARGET_EXT = .dll
} else:unix {
    target.path = /usr/lib
    INSTALLS += target
}
