QT += serialport
INCLUDEPATH += $$system(pwd)/src

HEADERS += \
    $$PWD/src/qfirmata.h \
    $$PWD/src/qfirmatadefines.h
SOURCES += \
    $$PWD/src/qfirmata.cpp

DISTFILES += \
    $$PWD/README.md
