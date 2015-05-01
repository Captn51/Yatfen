#-------------------------------------------------
#
# Project created by QtCreator 2015-05-01T16:15:48
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Yatfen
TEMPLATE = app

QMAKE_CXXFLAGS += -std=c++11


SOURCES += \
    main.cpp \
    yatfen.cpp \
    yatzee.cpp \
    sm.cpp

HEADERS  += \
    yatfen.h \
    yatzee.h \
    sm.h

RC_FILE += \
    yatfen.rc
