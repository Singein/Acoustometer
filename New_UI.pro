#-------------------------------------------------
#
# Project created by QtCreator 2016-12-11T21:38:19
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = New_UI
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp \
    portagent.cpp \
    settings.cpp \
    deviceparameter.cpp \
    database.cpp \
    toexcel.cpp \
    crccheck.cpp

HEADERS  += widget.h \
    portagent.h \
    settings.h \
    deviceparameter.h \
    database.h \
    toexcel.h \
    crccheck.h

FORMS    += widget.ui \
    settings.ui \
    deviceparameter.ui \
    toexcel.ui
QT += serialport
QT += sql
QT += axcontainer
