include(../common.pri)

TARGET = QtItems
TEMPLATE = lib
VERSION = 0.0.1

DEFINES += QI_LIBRARY

SOURCES += QiAPI.cpp \
    utils/Range.cpp \
    utils/ranges/RangesBasic.cpp

HEADERS +=  QiAPI.h \
    utils/Signal.h \
    utils/CellID.h \
    utils/Range.h \
    utils/ranges/RangesBasic.h

win32 {
    TARGET_EXT = .dll
} else:unix {
    target.path = /usr/lib
    INSTALLS += target
}
