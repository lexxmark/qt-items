include(../common.pri)

QT += core gui widgets

TARGET = qi-demos
TEMPLATE = app

SOURCES +=  main.cpp\
            MainWindow.cpp

HEADERS  += MainWindow.h

FORMS    += MainWindow.ui

INCLUDEPATH += $$ROOT_DIR/src/
LIBS += -L$$DESTDIR -lqt-items

#win32 {
#} else:unix {
#    QMAKE_LFLAGS += -Wl,-rpath,$$BIN_DIR
#}
