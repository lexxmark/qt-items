TEMPLATE   = subdirs
SUBDIRS   += src\
             tests\
             demos

src.file = src/qt-items-lib.pro

tests.depends = src
demos.depends = src
