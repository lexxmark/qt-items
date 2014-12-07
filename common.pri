ROOT_DIR = $$PWD

CONFIG+=c++11

win32:QMAKE_CXXFLAGS += /wd4100 /wd4065 -D_CRT_SECURE_NO_WARNINGS
unix:QMAKE_CXXFLAGS += -Wno-c++0x-compat -Wno-unused-parameter -Wno-unused-function

DESTDIR = $$OUT_PWD/../bin/
