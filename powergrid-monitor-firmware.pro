TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    app.cpp \
    histogram.cpp \
    screen.cpp

DISTFILES += \
    powergrid-monitor-firmware.ino

HEADERS += \
    app.h \
    histogram.h \
    screen.h
